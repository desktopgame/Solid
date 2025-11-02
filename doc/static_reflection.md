# 静的リフレクション
このプロジェクトでは静的リフレクションと呼ばれる技術を採用しています。  
端的に述べると、あらかじめ用意されたメタデータからソースコードを生成するというものです。  
以下、この技術を採用するにいたった背景と得られたメリットについて述べます。

## 課題
DirectX12とそれ以前、例えばDirectX11などでは大きく違う点が存在します。  
それがパイプラインステートです。  
OpenGLなどもそうですが、昔のDirectXはデプスステートやブレンドステートを単独で切り替えることができたのです。  
しかしいまでは違います。それらステートはパイプラインステートに統合されました。  
つまり切り替えるならパイプラインステート単位で切り替えるしかないのです。

しかし、パイプラインステートを切り替えるということはありとあらゆるものが切り替わるということです。
* 頂点入力レイアウト
* ルートシグネチャ（ユニフォームのレイアウト。何番目になにが入るかなど）
* ブレンドステート
* デプスステート
* シェーダー

そして、パイプラインステートは描画前に既に確定していなければなりません。  
これにより、愚直に実装すると些細な違いしかないバリエーションでも大量にパイプラインステート生成のコードを書くことになり、コードの重複を引き起こしてしまいます。

## 解決方法
プロジェクトの中にメタファイルがあり、ここからC++のソースコードを生成します。  
これ自体は単なるデータ定義に過ぎず、これを実行時に適切なパイプラインステートに変換する処理が存在します。  
このロジックが一つだけ存在し、コードの重複はありません。（とはいえ、多少の場合分けは存在します）

たとえば、ただ色を出力するだけの2Dシェーダーのメタファイルはこんな感じです。
```.meta
InputLayout=Vertex2D
PrimitiveType=Triangles
IsWireframe=false
InstanceBufferCount=0
UseGeometryShader=false
UseComputeShader=false
VS.UniformCount=2
VS.Uniform[0]=UCamera
VS.Uniform[1]=UVector4
VS.Code=Color2D.vs
PS.UniformCount=0
PS.Code=Color2D.ps
```

これが生成されるC++のコードになります。
```.cpp
Program {
    // inputLayout
    Reflect::InputLayout::Vertex2D,
    // instanceBufferLayout
    std::vector<Reflect::InstanceBufferType> {
    },
    // primitiveType
    Reflect::PrimitiveType::Triangles,
    // isWireframe
    false,
    // stencil
    Stencil { false, false, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilOp::Keep, Reflect::StencilFunc::Never },
    // vs
    "ここにシェーダーコード..."
    ,
    // vsUniforms
    std::vector<Uniform> {
        Uniform { sizeof(Reflect::UCamera), Uniform::Type::CBV },
        Uniform { sizeof(Reflect::UVector4), Uniform::Type::CBV },
    },
    // gs
    nullptr,
    // gsUniforms
    std::vector<Uniform> {
    },
    // ps
    "ここにシェーダーコード..."
    ,
    // psUniforms
    std::vector<Uniform> {
    },
    // cs
    nullptr,
    // csUniforms
    std::vector<Uniform> {
    },
}
```

現時点で17種類のシェーダーがこの仕組みで管理されています。  
まだ実装されていませんが、マクロやプリプロセスを実装して独自シェーダー言語を搭載すればさらにシェーダーの多様性に対して頑強なパイプラインを設計できると感じています。  

課題としては、グローバルライトやポイントライト、GBufferはこの仕組みの外でビルトインとして実装されている点があります。  
ここは描画エンジンがさらに汎用を目指すのか具体的な特定の用途にフォーカスするのかで分かれますが、まだ決めかねています。
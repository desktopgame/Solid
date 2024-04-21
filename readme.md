# Solid
`Solid` は DirectX12 で実装されている3Dゲームです。

## セットアップ
リポジトリをクローンしてから一度だけ行う必要のある作業です。  
いくつかのライブラリを `vcpkg` から取得しており、それらはサブモジュールとしてぶら下がる構造になっています。
まずは `vcpkg` を初期化し、必要なライブラリをインストールしてください。
````bat
cd src\Solid\mod\vcpkg
bootstrap-vcpkg.bat
vcpkg install directxtex
````

ここから先のセットアップ手順は開発環境により異なります。

### VisualStudio Code
VisualStudio Code の場合、 `Solid.code-workspace` を開けばそのまま適切な設定が有効化されます。  
VisualStudio Code 向けの設定はリポジトリにコミットされているためです。  
（使っている人が多いだろうと見込んですぐセットアップできるようしてあります。）  
開いた時点で必要な拡張機能がインストールされていなかった場合、ワークスペース設定により必要な拡張機能がレコメンドされます。

### CLion
CLionは公式にCMakeをサポートしているので、基本的には公式の使い方に従えば開くことができます。

* リポジトリのルートディレクトリを開く
* ツールチェイン設定より `clang++`, `lldb` を使うように変更
    * ![ツールチェイン設定画面](doc/images/CLion_toolchain.png)
* 実行時ディレクトリをリポジトリルートに変更
    * ![実行時ディレクトリ設定](doc/images/CLion_cwd.png)
* [公式ドキュメント](https://pleiades.io/help/clion/reloading-project.html#manual-reload)を参考に `CMakeLists` を読み込む
* うまくいかなかったら `File>Invalidate Caches...` を実行して最初からやり直す

## ビルド

### VisualStudio Code
````bat
cd Solid
cmake -S src -B build -G "Unix Makefiles"
make -C build
````

### CLion
CLionの場合、 `App` という名前になっている構成で実行ファイルをビルドできます。

## テスト
`src/App/mod` 以下の一部サブモジュールにはテストが書かれています。
````bat
cd Solid
make -C build Math-Test
````
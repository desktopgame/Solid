# Solid
`Solid` は DirectX12 で実装されている3Dゲームです。

## セットアップ
リポジトリをクローンしてから一度だけ行う必要のある作業です。  
いくつかのライブラリを `vcpkg` から取得しており、それらはサブモジュールとしてぶら下がる構造になっています。
まずは `vcpkg` を初期化し、必要なライブラリをインストールしてください。
````bat
cd src\App\mod\vcpkg
bootstrap-vcpkg.bat
vcpkg install directxtex
vcpkg install picojson
vcpkg install freetype
vcpkg install imgui[core,dx12-binding,win32-binding]:x64-windows
````

ここから先のセットアップ手順は開発環境により異なります。

### VisualStudio Code
VisualStudio Code の場合、 `Solid.code-workspace` を開けばそのまま適切な設定が有効化されます。  
VisualStudio Code 向けの設定はリポジトリにコミットされているためです。  
（使っている人が多いだろうと見込んですぐセットアップできるようしてあります。）  
開いた時点で必要な拡張機能がインストールされていなかった場合、ワークスペース設定により必要な拡張機能がレコメンドされます。

コード補完が働かない場合、一度ビルドを実行してみてください。  
このリポジトリではCMakeがビルド時に生成する `compile_commands.json` を利用してコード補完を行います。

### VisualStudio
VisualStudio の場合、CMakeでソリューションとプロジェクトを生成する必要があります。
````
cd Solid
cmake -S src -B build -G "Visual Studio 17 2022"
````

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
cmake -S src -B build -G "Ninja"
ninja -C build
````

### VisualStudio
VisualStudioの場合、 `App` という名前になっているプロジェクトで実行ファイルをビルドできます。

### CLion
CLionの場合、 `App` という名前になっている構成で実行ファイルをビルドできます。

## テスト
アプリケーション本体、そして一部サブモジュールについてはテストが書かれています。
````bat
cd Solid
ninja -C build Math-Test
ninja -C build Graphics-Test
ninja -C build Utils-Test
````

全てのテストを実行するには、以下コマンドを実行します。  
ただしこれを実行するとアプリケーションのテストも実行されます。
````bat
ninja -C build Test
````

逆に、アプリケーションのテストだけを実行したい場合は以下を実行します。
````bat
ctest --test-dir build/App -R "^Test$" -V
````
# マルチスレッドレンダリング
このプロジェクトではマルチスレッドレンダリングと呼ばれる技術を採用しています。  
端的に述べると、CPUとGPUを同時に仕事をさせることで1フレームの処理時間を短縮するというものです。  
以下、この技術を採用するにいたった背景と得られたメリットについて述べます。

## 課題
シンプルに実装すると、描画の流れは以下のようになります。

* CPUがその1フレームにおける `Update()` を実行する。ユーザーの入力を処理し、物理演算を実行する。
* CPUがGPUに対して描画コマンドを投げための `Draw()` を実行する。
* 描画コマンドを全て投げたら `Present()` を実行し、描画の終了を待機する。

この例では、1フレームの処理時間がCPUの時間+GPUの処理時間になります。    
しかし、CPUとGPUを同時に処理させることができればもっと処理時間を稼げるはず、というのがこのアイデアです。

## 解決方法
マルチスレッド版における描画の流れは以下になります。

* 前のフレームで積まれた描画コマンドについて、 `Present()` を実行する。
* CPUがその1フレームにおける `Update()` を実行する。ユーザーの入力を処理し、物理演算を実行する。
* 描画の終了を待機する。
* CPUがGPUに対して描画コマンドを投げための `Draw()` を実行する。

この新しい方式では、前のフレームの描画と今のフレームの更新が同時に行われます。  
そのため、以前の方式よりも処理時間を短縮できます。

## 実装方法
実際のコードは Surface.cpp に存在しますが、かいつまんで説明します。  
この描画エンジンではユーザー向けに見せるあらゆる描画処理はコマンドとしてラップされています。  
このコマンドというのは DirectX12 の世界のコマンドではありません。
以下、コマンドの一例です。

```.h
class ICommand;
class BeginGuiCommand;
class EndGuiCommand;
class Begin3DCommand;
class End3DCommand;
class Begin2DCommand;
class End2DCommand;
class BeginBatchCommand;
class EndBatchCommand;
class PresentCommand;
class SyncCommand;
class StencilRefCommand;
class StencilClearCommand;
class RenderCommand1;
class RenderCommand2;
class RenderCommand3;
class UniformVSCommand;
class UniformGSCommand;
class UniformPSCommand;
class UniformCSCommand;
class WaitCommand;
class ExitCommand;
```

これをキューに積んでいき、描画スレッドは順次取り出して実行します。  
また、コマンドはプール化されているので、毎フレームアロケーションして捨てるわけではありません。
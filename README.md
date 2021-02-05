![demo](demo.gif)

# Gravity Desktop  
**※このプログラムには新しいバージョン [GravityDesktop2](https://github.com/wakewakame/GravityDesktop2) があります。**

大学1年のときに作成した、デスクトップにあるアイコンが落下運動をするジョークソフトです。

### 仕組み
デスクトップの画面を再現したウィンドウを全画面に表示することでアイコンが落下しているように見せています。
具体的な処理の流れは以下の通りです。  

1. Win32APIを用いてデスクトップ上の全てのアイコンの座標を取得する
2. 1の座標を用いてBox2Dの仮想空間の中に長方形を生成する
3. Windowsの隠しAPIであるDwmGetDxSharedSurfaceを用いてデスクトップのアイコンをαチャンネル付きでキャプチャする
4. 2で生成した落下する長方形に、3で取得したアイコンのキャプチャを貼り付ける
5. ウィンドウに対して送られてくるマウスイベントなどを座標変換したのちにデスクトップに転送する
6. 3に戻る

### コンパイル済みファイルのダウンロード  
[Gravity Desktop v1.0](https://github.com/wakewakame/GravityDesktop/releases)  

### 動作条件
OS:Windows 10 64bit  

### 使用方法
premake5を[https://premake.github.io](https://premake.github.io)からダウンロード  
ダウンロードしたpremake5.exeをGravityDesktop\Box2D\Box2Dにコピー  
コマンドプロンプトでGravityDesktop\Box2D\Box2Dで"premake5.exe vs2017"と入力し実行  
GravityDesktop.slnを開く  
構成をRelease x64に変更  
mainプロジェクトを右クリックし、"スタートアッププロジェクトに設定"をクリック  
F5キーで実行  
Escで終了  

### 使用ライブラリ  
Box2D:[https://github.com/erincatto/Box2D](https://github.com/erincatto/Box2D)  
DirectX Tool Kit:[https://github.com/Microsoft/DirectXTK](https://github.com/Microsoft/DirectXTK)  

![demo](demo.gif)

# Gravity Desktop  
**※このプログラムには新しいバージョン [GravityDesktop2](https://github.com/wakewakame/GravityDesktop2) があります。**

大学1年のときに作成した、デスクトップにあるアイコンが落下運動をするジョークソフトです。

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

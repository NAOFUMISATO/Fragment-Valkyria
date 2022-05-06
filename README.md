# Fragment-Valkyria

AMG冬季共同制作3Dゲーム作品

ジャンル：3Dシューティングアクション

■ プログラム環境

OS : Windows10

開発環境 : Visual Studio 2019

開発言語 : C++11~20

使用ライブラリ

：DXライブラリ(https://dxlib.xsrv.jp/)

：nlohmann.json(https://github.com/nlohmann/json/)



■ プログラムファイル(cpp / h)の文字コードについて
DXライブラリ が Shift_JIS で作成されておりヘッダーをincludeする都合上自身の
プログラムファイルを UTF-8 にしてしまうとVisual Studio の方がインテリセンス
機能などが誤動作を起こすのでShift_JIS に統一しております。



■ プログラムについて
 本プログラムは、自作した AppFrame というフレームワーク部分とそのAppFrame
を使ってゲームロジック部分を組んだ Game の2 つの Visual Studioプロジェクト
に分かれています。


 WinMain は FragmentValkyria.vcxproj の FragmentValkyria\ソースファイル\WinMain.cpp にて実装
しています。


■AppFrame及びFragmentValkyriaクラスダイアグラム
「FragmentValkyriaクラスダイアグラム.pdf」ファイル参照。↓

[FragmentValkyriaクラスダイアグラム.pdf](https://github.com/NAOFUMISATO/Fragment-Valkyria/files/8641152/FragmentValkyria.pdf)

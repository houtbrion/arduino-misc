# Arduino-DHT22-RTC-SPP
RTCで一定周期でArduinoを起こし，DHT22を使って温度と湿度を測定し，
USBホストシールドの先につながったbluetoothアダプタ，もしくは
秋月電子通商製のRN42開発モジュールを使って
送信する
Arduino用プログラム

# 作った経緯など
以前，DHT11とLM35DZを利用した温度・湿度計を作成したところ，DHTの精度(特に温度)が
非常に悪いことと，測定回路をシールドにして1ヶ月弱程度連続運転したところ，Arduino
本体からシールドに上げる熱で，温度の測定値が非常に高くなってしまったため，
測定が不要な間は，CPUを省電力モードに設定したくなった．

CPUのウォッチドッグタイマはタイマが発火する期間が非常に短く(最大数秒)あまり
省電力的に意味がないため，RTCを使うことにした．

その時，手頃なarduino用RTCを探した所，[8564NB][rtc]を秋月電子の通販ページで
発見したため，適当なライブラリを探した所，[きむ茶さんのページ][kimcha]に
行き着いたので，それを使ってまず練習してみたのが
[bluetoothなしの試作版][dht22rtc]．

[bluetoothなしの試作版][dht22rtc]も今回も，
センサもDHT11は非常に精度が悪かったので，とりあえず多少ましなものとして
[DHT22][DHT22]を秋月電子で購入し，adafruitの[DHT用のライブラリ][dht]を
使っている．

本来の目的は，データを収集してなにかをすることなので，データ収集のために，
[USBホストシールド][HostShield]と
[Bluetooth4.0+EDR/LE対応 USBアダプター][bluetooth]を使って
サーバに送信する．

USB接続のBluetoothドングルを使うと，メモリの消費量が大きく
Mega系のマシンでしか実行できないため，
シリアルポートと[RN42][RN42]を接続し，
Bluetoothのシリアルプロファイルで送信する機能も作った．


# 現在の問題と次のネタ
* [Arduino Mega ADK][adk]はUSBホスト機能が存在するのに，現状対応していない．
* USB接続のドングルの場合，プログラムサイズが大きすぎて，[Arduino Uno][Uno]で動かない．

とりあえず，[Arduino Mega ADK][adk]で簡易なテストをしてみた試したところ，
USBホストシールドのライブラリでADKのUSBポートは駆動できる模様．

# ライセンス
きむ茶さん，adafruit共に，ライブラリやサンプルプログラムにライセンスが明示されていないので
ライセンス条件は不明だけど，自分が作った分(そんなものあるのかというのは
おいておいて)に関しては，AS-ISのライセンスにしときます．

ライセンスが気になる用途に利用される方は，「adafruit」と「きむ茶さん」に
問い合わせてください．

# 動作環境
## 用意した部品
* 本体 : [Arduino Mega 2560][Mega2560]
* 温湿度センサ : [DHT22(秋月電子通商)][DHT22]
* 箱 : [Arduino用ケース(千石電商)][case]
* RTC : [3564NB(秋月電子通商)][rtc]
* USBインターフェース : [USBホストシールド][HostShield]
* Bluetoothインターフェース : [BluetoothR4.0+EDR/LE対応 USBアダプター][bluetooth]
* Bluetoothドングルの代わりに用いるBluetoothモジュール(RN-42使用Bluetooth無線モジュール評価キット) : [RN42][RN42]

## 回路図など
USBドングル版の回路図は以下の通り．
本来の回路は[Arduino Mega 2560][Mega2560]で作っていますが，
[USBホストシールド][HostShield]が入ると
回路図がややこしくなるため，
図面上は[Arduino Mega ADK][adk]となっています．

![ブレッドボード利用時のイメージ][breadboard]

![回路図][circuit]

詳細はソースに添付してあるfritzingの回路図を参照．

## 開発環境と各種ライブラリ
* [Arduino開発環境][ide] : バージョン1.6.4
* [DHTセンサのライブラリ][dht]
* [きむ茶さんのRTCライブラリを改造したもの][skRTClib]
* [USBホストシールド用ライブラリ][usbhost]

# 内部動作の特徴
タイマで待ち合わせしている休止状態の期間は，CPUを低電力モードにして
処理が止まる．休止期間のCPU状態を
どのモードにするかは，ソースを修正すれば変更可能

# 自分の環境で動作させる場合
以下の情報を参照して，ソースコードを変更してね :-P

## DHT関係
今回は，5番ピンにDHT22の出力をつないで，いるので
以下の通り．ピン番号やセンサを違うものにする場合は以下の`define文`を変更してね．

    /*
     * DHT関係の定義
     */
    #include <DHT.h>
    #define DHTPIN 5     // what pin we're connected to


    // Uncomment whatever type you're using!
    //#define DHTTYPE DHT11   // DHT 11 
    #define DHTTYPE DHT22   // DHT 22  (AM2302)
    //#define DHTTYPE DHT21   // DHT 21 (AM2301)


## RTCからの割込み関係
以下のソースは
[Arduino Mega 2560][Mega2560]
用の設定になってます．

    //
    // RTCからarduinoに割込みを上げる端子と割込み番号の指定
    // 本当なら，割込み番号を入れたら端子も機種毎の切り替えを効かせたいが，とりあえず固定指定
    //
    // 手持ちのMega2560では，INT0,1が動作しなかったため，大きい番号の割込みを利用
    #define INT_NUMBER 5
    #define PIN_NUMBER 18
    // UNOはINT0,1が動作するため，こちらを利用
    //#define INT_NUMBER 0
    //#define PIN_NUMBER 2


## 端末が眠る深さと眠る期間
今は，一番深く眠る設定になってます．

    //
    //　端末が眠る場合の眠りの深さの指定
    //
    //#define STANDBY_MODE SLEEP_MODE_IDLE
    //#define STANDBY_MODE SLEEP_MODE_ADC
    //#define STANDBY_MODE SLEEP_MODE_PWR_SAVE
    //#define STANDBY_MODE SLEEP_MODE_STANDBY
    #define STANDBY_MODE SLEEP_MODE_PWR_DOWN

眠る期間は，「単位」と「その倍数」で指定(現状は20秒)．

    //
    // 端末が眠る期間の指定
    //
    #define SLEEP_DURATION 10 //単位の倍数
    //#define SLEEP_UNIT 0 // 244.14us単位
    //#define SLEEP_UNIT 1 //15.625ms単位
    #define SLEEP_UNIT 2 //秒単位
    //#define SLEEP_UNIT 3 //分単位

## デバッグ関係
以下の*define文*を有効にすると，シリアルにメッセージが
出力される．

`#define USE_SERIAL`


## その他
    //
    // 安全対策など
    //
    int counter=0; //一定回数システムが動作したら端末を再起動させるためのカウンタ
    #define THRESHOLD 20  // 20回測定したらArduinoが再起動

# 参考文献
* きむ茶さん <http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/RTC.htm>
* AdafruitのDHTページ <https://learn.adafruit.com/dht>
* bluetoothなしの試作 <https://github.com/houtbrion/DHT22RTC>

<!--以下はリンクの定義-->
<!--参考文献-->
[kimcha]: <http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/RTC.htm> "きむ茶さん"
[dht]: <https://learn.adafruit.com/dht> "AdafruitのDHTページ"
[dht22rtc]: <https://github.com/houtbrion/DHT22RTC> "bluetoothなしの試作版"

<!--開発環境と各種ライブラリ-->
[ide]: <http://www.arduino.cc/en/Main/Software> "Arduino開発環境"
[dht]: <https://github.com/adafruit/DHT-sensor-library> "DHTセンサのライブラリ"
[skRTClibOrig]: <http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/skRTClib.lzh> "きむ茶さんのRTCライブラリ"
[skRTClib]: <https://github.com/houtbrion/skRTClib> "きむ茶さんのRTCライブラリを改造したバージョン"
[usbhost]: <https://github.com/felis/USB\_Host\_Shield\_2.0> "USBホストシールド用ライブラリ"

<!--ハード関連-->
[Uno]: <http://www.arduino.cc/en/Main/ArduinoBoardUno> "Arduino Uno"
[Mega2560]: <http://www.arduino.cc/en/Main/ArduinoBoardMega2560> "Arduino Mega 2560"
[M0pro]: <http://www.arduino.org/products/arduino-m0-pro> "Arduino M0 pro"
[adk]: <http://www.arduino.cc/en/Main/ArduinoBoardMegaADK?from=Main.ArduinoBoardADK> "Arduino Mega ADK"
[DHT22]: <http://akizukidenshi.com/catalog/g/gM-07002/> "DHT22(秋月電子通商)"
[case]: <https://www.sengoku.co.jp/mod/sgk\_cart/detail.php?code=EEHD-4CLA> "Arduino用ケース(千石電商)"
[rtc]: <http://akizukidenshi.com/catalog/g/gI-00233/> "3564NB(秋月電子通商)"
[HostShield]: <http://www.aitendo.com/product/10293> "USBホストシールド"
[bluetooth]: <http://buffalo.jp/product/peripheral/wireless-adapter/bsbt4d09bk/> "BluetoothR4.0+EDR/LE対応 USBアダプター"
[RN42]: <http://akizukidenshi.com/catalog/g/gK-07378/> "ＲＮ－４２使用　Ｂｌｕｅｔｏｏｔｈ無線モジュール評価キット"

<!--イメージファイル-->
[system]: Doc/fig/system.jpg "本体写真"
[breadboard]: Doc/fig/breadboard.jpg "ブレッドボード利用時の配線イメージ"
[circuit]: Doc/fig/circuit.jpg "回路図"




# DHTWebServerって何?
HTTPリクエストを受信すると，部屋の温度と湿度を測定するして返信するArduino

![本体写真][system]

# 使い方

センサを搭載して，本アプリをインストールしたArduinoのホスト名を
"aaa.example.com"
とすると
"http://aaa.example.com/"
でアクセスすると，「人に見やすい」形式で温度と湿度を表示するが，
"http://aaa.example.com/arienai(ここはなんでも良い)/"
でアクセスすると，機械でパーズが
容易な形式で温度と湿度を返答する．

# 動作環境

## 用意した部品
* 本体 : [Arduino Ethernet R3][ethernetR3]
* 温湿度センサ : [DHT11][DHT11]
* 温度センサ : [LM35DZ][LM35DZ]
* シリアルインターフェース : [スイッチサイエンス製シリアルUSB変換][FTDI]
* 箱 : [Arduino用ケース][case]

## ブレッドボードのイメージ

![ブレッドボード利用時のイメージ][breadboard]

## 回路図

![回路図][circuit]


## 開発環境と各種ライブラリ
* [Arduino開発環境][ide] : バージョン1.6.4
* [pingのライブラリ][ping]
* [DHTセンサのライブラリ][dht]
* [タイマライブラリ][timer]
* [NTPのライブラリ][ntp]

# 内部動作の特徴

## ネットワークの設定
DHCPでも，固定IPアドレスでもOKだが，*DHCP環境ではきちんとテストしていない*．

また，オフィスの環境がイーサネットのMACアドレスで認証をかける方式のため，
端末がパケットを出さないとネットワークに接続できない環境のため，端末起動時に
自分からルータに対してpingを行うようになっている．
もし，不要ならソースの中の*define文*で制御できる

## 時計の取り扱い
開発版作成中に，数日放置すると固まる現象が見られたため，夜中にリブートするようにした．そのため，起動時にNTPサーバに時刻を問い合わせて，本体の時計をあわせるコードを追加した．

なお，リブートの条件は，「稼働時間が1時間以上」かつ「本体の時計が夜中の1時を過ぎている」となっている．

## 温度と湿度の測定
LM35DZとDHT11の両方を搭載しているが，DHT11の温度測定の精度があまりに低いため，温度はLM35DZから取得している．
また，LM35DZの測定結果も安定しないため，数十回測定し，その*中央値*を採用するように
している．

# 自分の環境で動作させる場合
以下の情報を参照して，ソースコードを変更してね :-P

## 本体に合わせて変更する場所
### 本体のMACアドレス
`byte mac[] = { 0xaa, 0xaa, 0xaa, 0xaa, 0xaa, 0xaa }; //アドレスは手持ちのarduinoのものに変更すること`

### DHT11関係
`#define DHTPIN 2     // DHTが接続されているデジタルのピン番号`

    // 使っているセンサの種類の定義
    #define DHTTYPE DHT11   // DHT 11
    //#define DHTTYPE DHT22   // DHT 22  (AM2302)
    //#define DHTTYPE DHT21   // DHT 21 (AM2301)`

### LM35関係
    float v=5.0;            // MEGA2560やEthernet R3は5.0V駆動
    float offset = 0;       // センサの温度ズレを修正する変数
    int TempSensorPin = 0;  // センサ用のアナログI/Fは0番を利用

## ネットワーク環境に合わせて変更する場所
### DHCP関係
`boolean useDhcp = false;                             // DHCPで運用する場合は true に変更`

### IPアドレス
    IPAddress ip(192,168,0,201);
    IPAddress dnsServer(192,168,0,1);
    IPAddress gatewayAddress(192,168,0,1);
    IPAddress netMask(255,255,255,0);

### NTPサーバ
イントラネットの場合や，国内のntpサーバを参照したい場合は以下の部分を変更

`#define NTP_SERVER "pool.ntp.org"`

### PINGの設定
うちの事務所のように，端末からpingを起動時にかける場合は以下のdefineを有効化．

`#define USE_PING`

### 文字コード
生成するHTMLに日本語が含まれるため，文字コードの定義が必要．現状はプログラムのinoファイルの
文字コードと同じにする必要があるためUTF-8としている．

    //client.println(F("Content-Type: text/html; charset=iso-2022-jp")); // JIS
    //client.println(F("Content-Type: text/html; charset=shift\_jis")); // SJIS
    //client.println(F("Content-Type: text/html; charset=euc-jp")); // EUC
    client.println(F("Content-Type: text/html; charset=UTF-8")); // UTF-8
    //client.println(F("Content-Type: text/html"));  // 定義なし

### 繰り返しのタイミング
現状，ブラウザでアクセスすると10秒間隔で再アクセスする設定となっている．
この時間間隔を変更したい場合は，以下の場所を変更する．

`client.println(F("Refresh: 10"));  // refresh the page automatically every 10 sec`


## デバッグフラグ
デバッグ用のフラグをONにすると，シリアルコンソールにメッセージが
出力される．

`#define DEBUG // デバッグ目的で，各種情報をシリアルに出力する場合は有効化`

# 現在の問題
特になし

# 参考文献
ソースの先頭にも記述しているが，参考にした情報源は以下の通り．

* <http://openhardware.gridshield.net/home/arduino-snmp-temperature-sensor>
* <http://d.hatena.ne.jp/a10i/20110607/1307453448>
* <http://homepage3.nifty.com/sudamiyako/zk/AVR\_ADC/AVR\_ADC.html>
* <http://makining.com/okuzawats/dht11-temperature-humidity-20150218>
* <https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor>
* <http://mpu.seesaa.net/article/399685627.html>
* <http://blog.livedoor.jp/hardyboy/tag/LM35DZ>
* <http://makining.com/okuzawats/dht11-temperature-humidity-20150218>
* <http://qiita.com/masato/items/99e5dac91d13650e90a2>


<!--以下はリンクの定義-->

<!--開発環境と各種ライブラリ-->
[ide]: <http://www.arduino.cc/en/Main/Software> "Arduino開発環境"
[ping]: <http://playground.arduino.cc/Code/ICMPPing> "pingのライブラリ"
[dht]: <https://github.com/adafruit/DHT-sensor-library> "DHTセンサのライブラリ"
[timer]: <http://playground.arduino.cc/Code/Time> "タイマライブラリ"
[ntp]: <http://playground.arduino.cc/Code/NTPclient> "NTPのライブラリ"

<!--ハード関連-->
[ethernetR3]: <http://www.arduino.cc/en/Main/ArduinoBoardEthernet> "Arduino Ethernet"
[DHT11]: <http://akizukidenshi.com/catalog/g/gM-07003/> "温湿度センサ" 
[LM35DZ]: <http://akizukidenshi.com/catalog/g/gI-00116/> "温度センサ"
[case]: <https://www.sengoku.co.jp/mod/sgk\_cart/detail.php?code=EEHD-4CLA> "Arduino用ケース"
[FTDI]: <https://www.switch-science.com/catalog/1032/> "シリアルインターフェース"

<!--イメージファイル-->
[system]: Doc/fig/system.jpg "本体写真"
[breadboard]: Doc/fig/breadboard.jpg "ブレッドボード利用時の配線イメージ"
[circuit]: Doc/fig/circuit.jpg "回路図"


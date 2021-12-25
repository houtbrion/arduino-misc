# Dht22RtcSppRB
DHT22を使い測定した温度と湿度計をRedBearBLEシールドで送信するarduino用プログラム


# ライブラリ
とりあえず，作業に使った版を添付しておきますが，
正式には下記のライブラリを使ってね．
NordicSemiconductor arduino用BLE SDK
https://github.com/NordicSemiconductor/ble-sdk-arduino

RedBearLab nRF8001用ライブラリ
https://github.com/RedBearLab/nRF8001/

AdafruitのDHTセンサ用ライブラリ
https://github.com/adafruit/DHT-sensor-library

きむ茶さん
http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/RTC.htm

[skRTClib]: <https://github.com/houtbrion/skRTClib> "きむ茶さんのRTCライブラリを改造したバージョン"

# ライセンス
adafruitときむ茶さんのはライセンスが書いてない．
でも，
nordicのと，redbearのはBSD系のAS-ISのライセンス
だったので，うちのコードも同じにしときます．

# 参考文献
ソースの先頭にも記述しているが，参考にした情報源は以下の通り．
* きむ茶さん <http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/RTC.htm>
* AdafruitのDHTページ <https://learn.adafruit.com/dht>

<!--以下はリンクの定義-->
<!--参考文献-->
[kimcha]: <http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/RTC.htm> "きむ茶さん"
[dht]: <https://learn.adafruit.com/dht> "AdafruitのDHTページ"

<!--開発環境と各種ライブラリ-->
[ide]: <http://www.arduino.cc/en/Main/Software> "Arduino開発環境"
[dht]: <https://github.com/adafruit/DHT-sensor-library> "DHTセンサのライブラリ"
[skRTClibOrig]: <http://www.geocities.jp/zattouka/GarageHouse/micon/Arduino/RTC/skRTClib.lzh> "きむ茶さんのRTCライブラリ"
[skRTClib]: <https://github.com/houtbrion/skRTClib> "きむ茶さんのRTCライブラリを改造したバージョン"

<!--ハード関連-->
[Uno]: <http://www.arduino.cc/en/Main/ArduinoBoardUno> "Arduino Uno"
[Mega2560]: <http://www.arduino.cc/en/Main/ArduinoBoardMega2560> "Arduino Mega 2560"
[M0pro]: <http://www.arduino.org/products/arduino-m0-pro> "Arduino M0 pro"
[DHT22]: <http://akizukidenshi.com/catalog/g/gM-07002/> "DHT22(秋月電子通商)"
[case]: <https://www.sengoku.co.jp/mod/sgk\_cart/detail.php?code=EEHD-4CLA> "Arduino用ケース(千石電商)"
[rtc]: <http://akizukidenshi.com/catalog/g/gI-00233/> "3564NB(秋月電子通商)"

<!--イメージファイル-->
[system]: Doc/fig/system.jpg "本体写真"
[breadboard]: Doc/fig/breadboard.jpg "ブレッドボード利用時の配線イメージ"
[circuit]: Doc/fig/circuit.jpg "回路図"




/*
  LM35DZを用いた温度測定(0℃～100℃)
  シリアルモニターに摂氏で表示させるのと，httpでのアクセスで答える
  参考URL
  
httpサーバおよび，温度センサ関連
http://openhardware.gridshield.net/home/arduino-snmp-temperature-sensor
http://d.hatena.ne.jp/a10i/20110607/1307453448
http://homepage3.nifty.com/sudamiyako/zk/AVR_ADC/AVR_ADC.html
http://makining.com/okuzawats/dht11-temperature-humidity-20150218
https://learn.adafruit.com/dht/connecting-to-a-dhtxx-sensor
http://mpu.seesaa.net/article/399685627.html
http://blog.livedoor.jp/hardyboy/tag/LM35DZ
http://makining.com/okuzawats/dht11-temperature-humidity-20150218
http://qiita.com/masato/items/99e5dac91d13650e90a2

pingのライブラリ
http://playground.arduino.cc/Code/ICMPPing

DHTセンサのライブラリ
https://github.com/adafruit/DHT-sensor-library

タイマライブラリ
http://playground.arduino.cc/Code/Time

NTP
http://playground.arduino.cc/Code/NTPclient
*/

/* 固定IP設定の時に，pingで設定を確認する機能を有効にする場合に定義する */
#define USE_PING


/*
 * デバッグ関連
 */
//#define DEBUG // デバッグ目的で，各種情報をシリアルに出力する場合は有効化

/* ネットワーク関係のインクルード */
#include <SPI.h>
#include <Ethernet.h>
#include <EthernetUdp.h>

#ifdef USE_PING
#include <ICMPPing.h>
#endif /* USE_PING */


/* 時計関係 */
#include <Time.h>

/* DHTセンサ関係のインクルード */
#include "DHT.h"


/*
 * DHT11関係の定義
 */
#define DHTPIN 2     // DHTが接続されているデジタルのピン番号

// 使っているセンサの種類の定義
#define DHTTYPE DHT11   // DHT 11 
//#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Connect pin 1 (on the left) of the sensor to +5V
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//DHT dht(DHTPIN, DHTTYPE, 30);

#define DHTSLEEP 1000 // DHT11の測定周波数は1Hz

/*
 * LM35(温度センサ)関連の定数の定義
 */
float v=5.0;            // MEGA2560やEthernet R3は5.0V駆動
float tempC   = 0;      // 摂氏値( ℃ )
float offset = 0;       // センサの温度ズレを修正する変数

#define LOOPNUMBER 50   // LM35の測定値を安定させるため，測定する回数
#define MEDIAN 25       // 繰り返した回数の半分(中央値)を測定値として採用する

/*
 * 配線関連の定数
 */
int TempSensorPin = 0;  // センサ用のアナログI/Fは0番を利用

/*
 * ネットワーク関係の定数
 */
byte mac[] = { 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff }; //アドレスは手持ちのarduinoのものに変更すること
// DHCPでのアドレス取得失敗時の対策や，長時間経過後のアドレス再割当て等は対応していない
boolean useDhcp = false;                             // DHCPで運用する場合は true に変更

/* 以下は固定IP運用の場合の変数なので適宜変更して使用すること */
IPAddress ip(192,168,0,201);
IPAddress dnsServer(192,168,0,1);
IPAddress gatewayAddress(192,168,0,1);
IPAddress netMask(255,255,255,0);

#ifdef USE_PING
SOCKET pingSocket = 0;
char buffer [256];
#endif /* USE_PING */

#define NTP_SERVER "pool.ntp.org"

//
// 時計関係
//
unsigned long bootTime;

/*
 * プログラム本体
 */
#ifdef USE_PING
ICMPPing ping(pingSocket, (uint16_t)random(0, 255));
#endif /* USE_PING */

EthernetServer server(80); // サーバオブジェクトの定義． 引数はポート番号

void setup() {
#ifdef DEBUG
  // シリアル通信速度
  Serial.begin(9600);
  Serial.println(F("Serial setup done"));
#endif
  // DHT11センサの初期化
  dht.begin();
#ifdef DEBUG
    Serial.println(F("sensor setup done"));
#endif
  // MACアドレスとIPアドレスの設定
  // 参考URL http://arduino.cc/en/Reference/EthernetBegin
  if (useDhcp) {
    if (Ethernet.begin(mac)==0) {
       reboot();
    }
  } else {
    Ethernet.begin(mac, ip, dnsServer, gatewayAddress, netMask);
#ifdef DEBUG
    Serial.println(F("ethernet setup done"));
#endif
#ifdef USE_PING
    // ルータのアドレスにping
    // 参考URL http://playground.arduino.cc/Code/ICMPPing
    ICMPEchoReply echoReply = ping(gatewayAddress, 4);
#ifdef DEBUG
    if (echoReply.status == SUCCESS) {
      Serial.println(F("Ping to Gateway address : success"));
    } else {
      Serial.println(F("Ping to Gateway address : fail"));
    }
#endif
#endif /* USE_PING */
  }
#ifdef DEBUG
  Serial.println(F("network setup done"));
#endif
  EthernetUDP udp;
  bootTime = ntpUnixTime(udp);
  setTime(bootTime + (9 * 60 * 60)); // JSTで時計を保存
  bootTime += 9 * 60 * 60;
#ifdef DEBUG
  Serial.print(F("JST is "));
  Serial.print(year());
  Serial.print(F('/'));
  Serial.print(month());
  Serial.print(F('/'));
  Serial.print(day());
  Serial.print(F(' '));
  Serial.print(hour());
  Serial.print(F(':')); 
  Serial.print(minute());
  Serial.print(F(':')); 
  Serial.println(second());
  Serial.println();
#endif /* DEBUG */
  // サーバの実行
  server.begin();
#ifdef DEBUG
  Serial.println(F("http server start"));
#endif /* DEBUG */
}

void loop() {
  unsigned long currentTime =now();
  if ((1==hour())&&(3600 < currentTime - bootTime)){ // 稼働時間が1時間以上で，夜中一時だったらリブート
#ifdef DEBUG
    Serial.println(F("timer fire"));
#endif /* DEBUG */
    reboot();
  }
  /*
   * httpリクエスト解析用
   */
  String httpRequestString="";
  String httpBuffer="";
  // 接続してきたクライアントを示すオブジェクトを生成
  EthernetClient client = server.available();
  if (client) {
    delay(DHTSLEEP); // DHTは測定の周波数が非常に長いため，連続してアクセスできないように，待ち時間をはさむ
#ifdef DEBUG
    Serial.println(F("Connection established by a client"));
#endif
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read(); //一文字読み取り
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          boolean httpRequestClass = matchRequestString(httpRequestString);
          httpRequestString = "";
          // センサから値を取得して体感温度を計算
          // LM35温度読み取り  
          float temperatureEX = getLMTemperatureEX(TempSensorPin);
          // DHT11の温度湿度
          float h = getDHTHumidity();
          // 摂氏の温度取得
          float t = getDHTCTemperature();
          // 華氏の温度取得
          float f = getDHTFTemperature();
          // LM35の温度とDHT11の湿度から，体感温度計算
          float hi = dht.convertFtoC(dht.computeHeatIndex(dht.convertCtoF(temperatureEX), h));
          // HTTPのレスポンスのヘッダ生成
          client.println("HTTP/1.1 200 OK");
          //返答する文字コードの定義
          //client.println(F("Content-Type: text/html; charset=iso-2022-jp")); // JIS
          //client.println(F("Content-Type: text/html; charset=shift_jis")); // SJIS
          //client.println(F("Content-Type: text/html; charset=euc-jp")); // EUC
          client.println(F("Content-Type: text/html; charset=UTF-8")); // UTF-8
          //client.println(F("Content-Type: text/html"));  // 定義なし
          client.println(F("Connection: close"));  // the connection will be closed after completion of the response
          //client.println(F("Refresh: 5"));  // refresh the page automatically every 5 sec
          client.println(F("Refresh: 10"));  // refresh the page automatically every 10 sec
          client.println();
          client.println(F("<!DOCTYPE HTML>"));
          client.println(F("<html>"));
          // httpRequestが人手によるものか否かでHTMLの内容を切り替え
          if (httpRequestClass) {
            // 人が入力した場合
            time_t currentTime=now();
            if (isnan(h) || isnan(t) || isnan(f)) {
              client.println(F("LM35の測定値 : "));
              client.println(F("<br />"));
              client.print(F("摂氏 "));
              client.print(temperatureEX);
              client.println(F(" 度"));
              client.println(F("<br />"));
              client.println(F("<br />"));
              client.println(F("DHT11はエラーで測定できませんでした"));
              client.println(F("<br />"));
            } else {
              client.println(F("<head><title>室内の温度・湿度環境</title></head>"));
              client.println(F("<body>"));
              client.println(F("<h1>室内の温度・湿度環境</h1>"));
              client.println(F("<br />"));
              client.println(F("<br />"));
              client.println(F("<style>table {border-collapse: collapse;} td{border: solid 1px;padding: 0.5em;}</style>"));
              client.println(F("<table>"));
              client.println(F("<caption>arduinoの出力</caption>"));
              client.println(F("<tr>"));
              client.println(F("<td>LM35測定結果</td>"));
              client.println(F("<td colspan=\"3\">DHT11測定結果</td>"));
              client.println(F("<td rowspan=\"2\">体感温度(摂氏)</td>"));
              client.println(F("</tr>"));
              client.println(F("<tr>"));
              client.println(F("<td>温度(摂氏)</td>"));
              client.println(F("<td>温度(摂氏)</td>"));
              client.println(F("<td>温度(華氏)</td>"));
              client.println(F("<td>湿度(\%)</td>"));
              client.println(F("</tr>"));
              client.println(F("<tr>"));
              client.print(F("<td>"));client.print(temperatureEX);client.println(F("</td>"));
              client.print(F("<td>"));client.print(t);client.println(F("</td>"));
              client.print(F("<td>"));client.print(f);client.println(F("</td>"));
              client.print(F("<td>"));client.print(h);client.println(F("</td>"));
              client.print(F("<td>"));client.print(hi);client.println(F("</td>"));
              client.println(F("</tr>"));
              client.println(F("</table>"));
              client.println(F("<br />"));
              client.println(F("注意事項など :"));
              client.println(F("<br />"));
              client.println(F("1) DHT11は精度が低いので温度・湿度共に参考程度にしてください．"));
              client.println(F("<br />"));
              client.println(F("2) DHT11は秒1回しか測定できないので，測定の依頼をすると，1秒待たされます"));
              client.println(F("<br />"));
              client.println(F("3) 体感温度はLM35DZの温度とDHT11の湿度から計算しています．"));
              client.println(F("<br />"));
              client.println(F("<br />"));
              client.print(F("端末起動時間 : "));
              client.print(year(bootTime));
              client.print(F("年 "));
              client.print(month(bootTime));
              client.print(F("月 "));
              client.print(day(bootTime));
              client.print(F("日 "));
              client.print(hour(bootTime));
              client.print(F("時 "));
              client.print(minute(bootTime));
              client.print(F("分 "));
              client.print(second(bootTime));
              client.println(F("秒"));
              client.println(F("<br />"));
              client.print(F("現在時刻 : "));
              client.print(year(currentTime));
              client.print(F("年 "));
              client.print(month(currentTime));
              client.print(F("月 "));
              client.print(day(currentTime));
              client.print(F("日 "));
              client.print(hour(currentTime));
              client.print(F("時 "));
              client.print(minute(currentTime));
              client.print(F("分 "));
              client.print(second(currentTime));
              client.println(F("秒"));
              client.println(F("<br />"));
              client.println(F("</body>"));
            }
          } else {
            // 人でない場合
            // LM35の測定結果を出力
            client.print(F("LM35:C:"));
            client.print(temperatureEX);
            client.print(F(":"));
            // DHTの測定結果を出力
            client.print(F("DHT11:"));
            // DHTの出力がエラーか否かで切り替え
            if (isnan(h) || isnan(t) || isnan(f)) {
              client.println(F("C:0:F:0:H:0:HI:C:0"));
            } else {
              client.print(F("C:"));
              client.print(t);
              client.print(F(":F:"));
              client.print(f);
              client.print(F(":H:"));
              client.print(h);
              client.print(F(":HI:C:"));
              client.println(hi);
            }
            client.println(F("<br />"));
          }
          client.println(F("</html>"));
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
          if (httpBuffer.startsWith("GET ")) {
            // GET xxx  HTTP/1.1　の行だけを処理対象にする．
            // 「xxx」の部分だけを取り出す．
            httpRequestString = httpBuffer;
            httpRequestString.replace("GET ","");
            httpRequestString.replace("HTTP/1.1","");
            httpRequestString.replace(" ","");
            httpRequestString.replace("\t","");
          }
          httpBuffer = "";
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
          httpBuffer = httpBuffer + c;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
  }
}

//
// DHT11の温度と湿度読み取り関数
float getDHTCTemperature(){
  float t = dht.readTemperature(false);
  return t;
}
float getDHTFTemperature(){
  float t = dht.readTemperature(true);
  return t;
}
float getDHTHumidity(){
  float h = dht.readHumidity();
  return h;
}

//
// LM35の温度読み取り関数
//
float getLMTemperatureEX(int pin){
  int buff[LOOPNUMBER];
  float tmp;
  for (int i=0 ; i < LOOPNUMBER ; i++){
    buff[i] = analogRead( pin );
  }
  sort(buff,LOOPNUMBER);
  tmp = ((v * buff[MEDIAN]) / 1023) * 100;
  tmp = tmp + offset;
  return tmp;
}
//
// データのソート
//
void sort(int a[], int size) {
    for(int i=0; i<(size-1); i++) {
        for(int o=0; o<(size-(i+1)); o++) {
                if(a[o] > a[o+1]) {
                    int t = a[o];
                    a[o] = a[o+1];
                    a[o+1] = t;
                }
        }
    }
}

/*
 * httpリクエストの解析関数
 *
 * 意図:
 * 人がブラウザでアクセスした時だけtrueにしたい
 * http://ホスト名(もしくはIPアドレス/ でアクセスした時は人がブラウザを使ったと見なす．
 * http://ホスト名(もしくはIPアドレス/の後ろにゴミがついていた場合は，マシンが自動で読み出しに来たと見なす．
 *
 * 仕様 : 
 * httpクリエスとの文字列がある文字列("/"か"/favicon.ico")に一致したらtrue，それ以外はfalse
 */
#define HTTP_REQUEST_STRING "/"
boolean matchRequestString(String httpRequestString) {
  String request_url = HTTP_REQUEST_STRING;
  request_url = request_url + "favicon.ico";
  int comp_result1 = httpRequestString.compareTo(HTTP_REQUEST_STRING);
  int comp_result2 = httpRequestString.compareTo(request_url);
  if ((comp_result1 == 0) || (comp_result2 == 0)) {
    // どちらかの結果が1なら，定義したURL文字列と一致した
    return true;
  } else {
    // それ以外の場合は，一致していない
    return false;
  }
}

/* Arduino playgroundのNTPクライアント
 * © Francesco Potortì 2013 - GPLv3 - Revision: 1.13
 *
 * Send an NTP packet and wait for the response, return the Unix time
 *
 * To lower the memory footprint, no buffers are allocated for sending
 * and receiving the NTP packets.  Four bytes of memory are allocated
 * for transmision, the rest is random garbage collected from the data
 * memory segment, and the received packet is read one byte at a time.
 * The Unix time is returned, that is, seconds from 1970-01-01T00:00.
 */
unsigned long inline ntpUnixTime (UDP &udp)
{
  static int udpInited = udp.begin(123); // open socket on arbitrary port

  const char timeServer[] = NTP_SERVER;  // NTP server

  // Only the first four bytes of an outgoing NTP packet need to be set
  // appropriately, the rest can be whatever.
  const long ntpFirstFourBytes = 0xEC0600E3; // NTP request header

  // Fail if WiFiUdp.begin() could not init a socket
  if (! udpInited)
    return 0;

  // Clear received data from possible stray received packets
  udp.flush();

  // Send an NTP request
  if (! (udp.beginPacket(timeServer, 123) // 123 is the NTP port
	 && udp.write((byte *)&ntpFirstFourBytes, 48) == 48
	 && udp.endPacket()))
    return 0;				// sending request failed

  // Wait for response; check every pollIntv ms up to maxPoll times
  const int pollIntv = 150;		// poll every this many ms
  const byte maxPoll = 15;		// poll up to this many times
  int pktLen;				// received packet length
  for (byte i=0; i<maxPoll; i++) {
    if ((pktLen = udp.parsePacket()) == 48)
      break;
    delay(pollIntv);
  }
  if (pktLen != 48)
    return 0;				// no correct packet received

  // Read and discard the first useless bytes
  // Set useless to 32 for speed; set to 40 for accuracy.
  const byte useless = 40;
  for (byte i = 0; i < useless; ++i)
    udp.read();

  // Read the integer part of sending time
  unsigned long time = udp.read();	// NTP time
  for (byte i = 1; i < 4; i++)
    time = time << 8 | udp.read();

  // Round to the nearest second if we want accuracy
  // The fractionary part is the next byte divided by 256: if it is
  // greater than 500ms we round to the next second; we also account
  // for an assumed network delay of 50ms, and (0.5-0.05)*256=115;
  // additionally, we account for how much we delayed reading the packet
  // since its arrival, which we assume on average to be pollIntv/2.
  time += (udp.read() > 115 - pollIntv/8);

  // Discard the rest of the packet
  udp.flush();

  return time - 2208988800ul;		// convert NTP time to Unix time
}
//
// 本体をリセットする関数
//
void reboot() {
  asm volatile ("  jmp 0");
}

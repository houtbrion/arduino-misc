/*
 * DHT系のセンサを使って，温度と湿度を測定し，Bluetoothで送信するプログラム
 * RTCを使って，一定時間間隔で測定と送信を実行する．
 * 測定していない期間はArduinoはスリープ状態となり，
 * RTCの割込みで処理が再開する
 */

#define USE_SERIAL
#define USE_RTC
#define USE_SOFT_SERIAL
//#define USE_USB


#ifdef USE_SOFT_SERIAL
#include <SoftwareSerial.h>
SoftwareSerial mySerial(2, 3);
#endif /* USE_SOFT_SERIAL */
/*
 * USBホストシールドでBluetoothを使うための各種定義
 */

#include <SPP.h>
//#include <usbhub.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#include <SPI.h>
#endif

#ifdef USE_USB
USB Usb;
//USBHub Hub1(&Usb); // Some dongles have a hub inside

BTD Btd(&Usb); // You have to create the Bluetooth Dongle instance like so
/* You can create the instance of the class in two ways */
SPP SerialBT(&Btd); // This will set the name to the defaults: "Arduino" and the pin to "0000"
//SPP SerialBT(&Btd, "Lauszus's Arduino", "1234"); // You can also set the name and pin like so
#endif /* USE_USB */

/*
 * RTC8564用のライブラリを使うためのインクルードのロードと各種定義
 */
#ifdef USE_RTC
#include <Wire.h>
#include <skRTClib.h>
#include <avr/sleep.h>
/*
 * RTCからarduinoに割込みを上げる端子と割込み番号の指定
 * 本当なら，割込み番号を入れたら端子も機種毎の切り替えを効かせたいが，とりあえず固定指定
 */
// 手持ちのMega2560では，INT0,1が動作しなかったため，大きい番号の割込みを利用
#define INT_NUMBER 5
#define PIN_NUMBER 18
// UNOはINT0,1が動作するため，こちらを利用
//#define INT_NUMBER 0
//#define PIN_NUMBER 2
/*
 *　端末が眠る場合の眠りの深さの指定
 */
//#define STANDBY_MODE SLEEP_MODE_IDLE
//#define STANDBY_MODE SLEEP_MODE_ADC
//#define STANDBY_MODE SLEEP_MODE_PWR_SAVE
//#define STANDBY_MODE SLEEP_MODE_STANDBY
#define STANDBY_MODE SLEEP_MODE_PWR_DOWN

/*
 * 端末が眠る期間の指定
 */
#define SLEEP_DURATION 10 //単位の倍数
//#define SLEEP_UNIT 0 // 244.14us単位
//#define SLEEP_UNIT 1 //15.625ms単位
#define SLEEP_UNIT 2 //秒単位
//#define SLEEP_UNIT 3 //分単位
#endif /* USE_RTC */

#ifdef USE_SERIAL
#include <stdio.h>
#endif /* USE_SERIAL */

/*
 * DHT関係の定義
 */
#include <DHT.h>
#define DHTPIN 5     // what pin we're connected to

// DHTの種類の選択
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

// Adafruitの注意事項，よく読むこと
// Connect pin 1 (on the left) of the sensor to +5V  (3V動作のマシンでは，センサの電源端子には3Vを入力)
// NOTE: If using a board with 3.3V logic like an Arduino Due connect pin 1
// to 3.3V instead of 5V!
// Connect pin 2 of the sensor to whatever your DHTPIN is
// Connect pin 4 (on the right) of the sensor to GROUND
// Connect a 10K resistor from pin 2 (data) to pin 1 (power) of the sensor

// Initialize DHT sensor for normal 16mhz Arduino
// 普通のArduinoは以下の関数を利用
DHT dht(DHTPIN, DHTTYPE);
// NOTE: For working with a faster chip, like an Arduino Due or Teensy, you
// might need to increase the threshold for cycle counts considered a 1 or 0.
// You can do this by passing a 3rd parameter for this threshold.  It's a bit
// of fiddling to find the right value, but in general the faster the CPU the
// higher the value.  The default for a 16mhz AVR is a value of 6.  For an
// Arduino Due that runs at 84mhz a value of 30 works.
// Example to initialize DHT sensor for Arduino Due:
//
// Dueの例
//
//DHT dht(DHTPIN, DHTTYPE, 30);
//
// arduino M0 proの場合
//DHT dht(DHTPIN, DHTTYPE,18);

/*
 * 安全対策など
 */
int counter=0; //一定回数システムが動作したら端末を再起動させるためのカウンタ
#define KELVIN 273.15 //絶対温度と摂氏の変換に利用
#define THRESHOLD 20

/*
 * 以下，プログラム本体
 */
#ifdef USE_SERIAL
bool firstMessage = true;
static FILE uartout;

static int uart_putchar (char c, FILE *stream) {
  if (Serial.write(c) > 0) {
    return 0;
  } else {
    return -1;
  }
}
#endif /* USE_SERIAL */


/*
 * RTC(CLKOUT)からの外部割込みで実行される関数
 * loop()の中で割込みが起きたかどうかを判定するための
 * フラグを立てる．
 * 他の処理を書いても良いが，割込みを使わないものに限る．
 */

#ifdef USE_RTC
void InterRTC()
{
  skRTC.InterFlag = 1 ;
#ifdef USE_SERIAL
  Serial.println(F("Interupt"));
#endif /* USE_SERIAL */
}
#endif /* USE_RTC */

/*
 * 端末をスリープ状態に設定する関数
 */
#ifdef USE_RTC
void goodNight(int i) {
#ifdef USE_SERIAL
  Serial.println(F("  Good Night"));
#endif /* USE_SERIAL */
  delay(100);
  noInterrupts();
  set_sleep_mode(i);
  sleep_enable();
  interrupts();
  sleep_cpu();
  sleep_disable();
}
#endif /* USE_RTC */
/*
 * 長時間動作させた場合に，動作がおかしくなるのを防ぐため
 * たまにリセットする時に使う関数
 */
void software_Reset(){
#ifdef USE_SERIAL
  Serial.println(F(" RESET"));
#endif /* USE_SERIAL */
  asm volatile ("  jmp 0");
} 

/*
 * 初期化
 * USB, Bluetooth,センサの初期化を実施
 * 初期化に失敗したら，無限ループに入る
 */
void setup()
{
  int ans ;
#ifdef USE_SERIAL
  fdev_setup_stream (&uartout, uart_putchar, NULL, _FDEV_SETUP_WRITE);
  stdout = &uartout;
#endif /* USE_SERIAL */
#ifdef USE_RTC
  pinMode( PIN_NUMBER, INPUT);    // RTCからの割込みを読むのに利用
#endif /* USE_RTC */
#ifdef USE_SERIAL
  Serial.begin(9600) ;                    // シリアル通信の初期化
#endif /* USE_SERIAL */
#ifdef USE_SOFT_SERIAL
  mySerial.begin(9600);
#endif /* USE_SOFT_SERIAL */
#ifdef USE_RTC
  ans = skRTC.begin(PIN_NUMBER,INT_NUMBER,InterRTC,12,1,10,2,15,30,0) ;  // 2012/01/10 火 15:30:00 でRTCを初期化する
  if (ans == 0) {
#ifdef USE_SERIAL
    Serial.println(F("Successful initialization of the RTC")) ;// 初期化成功
#endif /* USE_SERIAL */
  } else {
#ifdef USE_SERIAL
    Serial.print(F("Failed initialization of the RTC ans=")) ; // 初期化失敗
    Serial.println(ans) ;
#endif /* USE_SERIAL */
    while(1) ;                         // 処理中断
  }
  skRTC.SetTimer(SLEEP_UNIT,SLEEP_DURATION) ;
#endif /* USE_RTC */
#ifdef USE_USB
  if (Usb.Init() == -1) {
#ifdef USE_SERIAL
    Serial.print(F("\r\nOSC did not start"));
#endif /* USE_SERIAL */
    while (1); //処理中断
  }
#ifdef USE_SERIAL
  Serial.print(F("\r\nSPP Bluetooth Library Started"));
#endif /* USE_SERIAL */
#endif /* USE_USB */
}

/*
 * メインのループ
 */
#ifdef USE_USB
void loop()
{
  Usb.Task(); // The SPP data is actually not send until this is called, one could call SerialBT.send() directly as well
  if (SerialBT.connected) {
#ifdef USE_SERIAL
    if (firstMessage) {
      // Bluetoothの接続に成功した場合だけ実行する処理
      firstMessage = false;
      Serial.println(F("Bluetooth connected"));
    }
#endif /* USE_SERIAL */
    float h = dht.readHumidity(); //湿度
    float t = dht.readTemperature(false); //摂氏
    float hi = dht.convertFtoC(dht.computeHeatIndex(dht.convertCtoF(t),h)); //体感温度
    if (isnan(t) || isnan(h)) {
#ifdef USE_SERIAL
      Serial.println(F("Failed to read from DHT"));
#endif /* USE_SERIAL */
      SerialBT.println(F("DHT22:K:0:H:0:HI:0"));
      SerialBT.flush();
    } else {
#ifdef USE_SERIAL
      Serial.print(F("Humidity: ")); 
      Serial.print(h);
      Serial.print(F(" %\t"));
      Serial.print(F("Temperature: ")); 
      Serial.print(t);
      Serial.println(F(" *C"));
      Serial.print(F("Heat Index : "));
      Serial.println(hi);
#endif /* USE_SERIAL */
      // //LM35:C:23.46:DHT11:C:32.00:F:89.60:H:42.00:HI:C:25.19
      // マイナス温度の処理の関係上，絶対温度で送信する
      t += KELVIN;
      hi += KELVIN;
      SerialBT.print(F("DHT22:K:"));
      SerialBT.print(t);
      SerialBT.print(F(":H:"));
      SerialBT.print(h);
      SerialBT.print(F(":HI:"));
      SerialBT.println(hi);
      SerialBT.flush();
    }
    counter++;
    if (counter==THRESHOLD) {
      // 一定時間以上動作したら一回端末をリセットする
      software_Reset();
    }
#ifdef USE_RTC
    goodNight(STANDBY_MODE);// 端末を眠らせる
    if (skRTC.InterFlag == 1) {  // 割込みが発生したか？
      skRTC.InterFlag = 0 ;                // 割込みフラグをクリアする
    }
#else /* USE_RTC */
    delay(30000);
#endif /* USE_RTC */
  } else {
#ifdef USE_SERIAL
    firstMessage = true;
#endif /* USE_SERIAL */
  }
}
#else /* USE_USB */
void loop()
{
  float h = dht.readHumidity(); //湿度
  float t = dht.readTemperature(false); //摂氏
  float hi = dht.convertFtoC(dht.computeHeatIndex(dht.convertCtoF(t),h)); //体感温度
#ifdef USE_SERIAL
  if (isnan(t) || isnan(h)) {
    Serial.println(F("Failed to read from DHT"));
  } else {
#else /* USE_SERIAL */
  if (!(isnan(t) || isnan(h))) {
#endif /* USE_SERIAL */
    // マイナス温度の処理の関係上，絶対温度で送信する
    t += KELVIN;
    hi += KELVIN;
#ifdef USE_SOFT_SERIAL
    mySerial.print(F("Humidity: ")); 
    mySerial.print(h);
    mySerial.print(F(" %\t"));
    mySerial.print(F("Temperature: ")); 
    mySerial.print(t);
    mySerial.println(F(" *C"));
    mySerial.print(F("Heat Index : "));
    mySerial.println(hi);
#endif /* USE_SOFT_SERIAL */
#ifdef USE_SERIAL
    Serial.print(F("Humidity: ")); 
    Serial.print(h);
    Serial.print(F(" %\t"));
    Serial.print(F("Temperature: ")); 
    Serial.print(t);
    Serial.println(F(" *C"));
    Serial.print(F("Heat Index : "));
    Serial.println(hi);
#endif /* USE_SERIAL */
    counter++;
    if (counter==THRESHOLD) {
      // 一定時間以上動作したら一回端末をリセットする
      software_Reset();
    }
#ifdef USE_RTC
    goodNight(STANDBY_MODE);// 端末を眠らせる
    if (skRTC.InterFlag == 1) {  // 割込みが発生したか？
      skRTC.InterFlag = 0 ;                // 割込みフラグをクリアする
    }
#else /* USE_RTC */
    delay(30000);
#endif /* USE_RTC */
  }
}
#endif /* USE_USB */

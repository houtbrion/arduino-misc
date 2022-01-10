

#define DEBUG
#define DEBUG_SERIAL_SPEED 115200

//#define USE_DHCP                 // IPアドレスをDHCPで行う (コメントアウトした場合は固定IP)

#define USE_DEFAULT_NETWORK_IF     // デフォルトのネットワークI/Fを使うか否かの指定． デフォルトが定義されない機種もあるので注意
/* デフォルトのNICを用いない場合は，以下の中から選択 */
//#define USE_WIFI_NINA              // デフォルトのNICを無視して，強制的にWiFi NINAライブラリを用いる場合
//#define USE_NORMAL_WIFI            // デフォルトのNICを無視して，強制的にWiFiのI/Fを使う場合
//#define USE_NORMAL_ETHERNET        //デフォルトのNICを無視して，強制的にイーサネットシールドを使う場合

//#define USE_CUSTOM_SERIAL

#define USE_HARD_RESET_PIN 15 // Espr one 32のD2 これを定義すると、ハードリセットプログラムがコンパイルされる
#define HARD_RESET_TIME 1000  // 1秒(定義の単位はms)

/*
 * telnetのポート番号定義
 */
#define PORT_NUMBER 23

/*
 * WiFiのI/Fを使う場合の定義
 */
#define SSID_STR "foo"
#define WIFI_PASS "bar"

/*
 * シリアル通信用I/Fのデフォルトパラメータの定義(デバッグ用ではない)
 */
#define DEFAULT_SERIAL_SPEED 115200
#define DEFAULT_SERIAL_PARAMETER SERIAL_8N1
//#define DEFAULT_SERIAL_PARAMETER SWSERIAL_8N1

/*
 * 一定時間入力がない場合に，通信を切断するので，その時間をミリ秒単位で定義
 */
#define INPUT_TIMEOUT 300000

/*
 * シリアル中継モードを抜けるために用いるエスケープコードの定義
 */
#define CTRL_CODE 0x11   // Ctrl-Q
//#define CTRL_CODE 0x12   // Ctrl-R
//#define CTRL_CODE 0x13   // Ctrl-S
//#define CTRL_CODE 0x14   // Ctrl-T

/*
 * telnetでのパスワード認証をPASSWD_RETRY_MAX回失敗するとconnectionは切る
 */
#define PASSWD_RETRY_MAX  3

/*
 * ここから下は基本的にカスタマイズ不要
 *  例外 :
 *  ・イーサネットシールドを用いる場合のMACアドレスの定義
 *  ・固定IP設定の場合のIPアドレスなど
 */
#define STATE_START         0
#define STATE_WAIT_CLIENT   1
#define STATE_INPUT_PASSWD  2
#define STATE_OPERATION     3
#define STATE_COMMUNICATION 4

#define CR 0x0d
#define LF 0x0a

#define PASSWD_CHAR_START 0x21
#define PASSWD_CHAR_END   0x7e

#define PASSWORD_LENGTH 16
#define PASSWORD1 "hoge"
#define PASSWORD2 "foo"
#define PASSWORD3 "bar"
#define PASSWD_NUM 3

#include <arduinoHardwareHelper.h>

#ifdef USE_DEFAULT_NETWORK_IF /* デフォルトで想定するネットワークI/Fの種類を定義 */

#if CPU_ARCH==AVR_ARCH /* AVRは基本Ethernetシールドを想定 */
#if HARDWARE_TYPE==ARDUINO_UNO_WIFI_DEV_ED
#define USE_WIFI_NINA
#else /* ARDUINO_UNO_WIFI_DEV_ED */
#define USE_NORMAL_ETHERNET
#endif /* ARDUINO_UNO_WIFI_DEV_ED */
#endif /* CPU_ARCH==AVR_ARCH */

#if CPU_ARCH==XTENSA_LX106_ARCH /* ESP8266 */
#define USE_NORMAL_WIFI
// ESP8266はSerial1とか2は利用できないのでsoftwareserial専用
#endif /* CPU_ARCH==XTENSA_LX106_ARCH (ESP8266) */

#if CPU_ARCH==XTENSA_LX6_ARCH /* ESP32 */
#define USE_NORMAL_WIFI
#endif /* CPU_ARCH==XTENSA_LX6_ARCH (ESP32) */

#if CPU_ARCH==SAMD_ARCH /* MKR系列など */
#if defined(ARDUINO_SAMD_NANO_33_IOT) || defined(ARDUINO_SAMD_MKRWIFI1010) || defined(ARDUINO_SAMD_MKRVIDOR4000)
#define USE_WIFI_NINA
#endif /* ARDUINO_SAMD_NANO_33_IOT || ARDUINO_SAMD_MKRWIFI1010 || ARDUINO_SAMD_MKRVIDOR4000 */
#endif /* CPU_ARCH==SAMD_ARCH  */

#endif /* USE_DEFAULT_NETWORK_IF */

#ifndef USE_CUSTOM_SERIAL /* デフォルトのシリアルを使うか否か */
#if CPU_ARCH==AVR_ARCH /* AVRはMegaを想定 */
#define COM_PORT Serial1  // GPIO19/18 RX/TX (Mega)
#endif /* CPU_ARCH==AVR_ARCH */

#if CPU_ARCH==XTENSA_LX106_ARCH /* ESP8266 */
// ESP8266はSerial1とか2は利用できないのでsoftwareserial専用
#define USE_SOFTWARE_SERIAL
#define SOFTWARE_SERIAL_TX 13
#define SOFTWARE_SERIAL_RX 12
#define COM_PORT comport
#endif /* CPU_ARCH==XTENSA_LX106_ARCH (ESP8266) */

#if CPU_ARCH==XTENSA_LX6_ARCH /* ESP32 */
#define COM_PORT Serial2  // GPIO16/17 RX/TX
#endif /* CPU_ARCH==XTENSA_LX6_ARCH (ESP32) */

#if CPU_ARCH==SAMD_ARCH /* MKR系列など */
#define COM_PORT Serial1  // GPIO13/14 RX/TX
#endif /* CPU_ARCH==SAMD_ARCH  */
#endif /* USE_CUSTOM_SERIAL */

/*
 * ソフトウェアシリアル関係の定義
 */
#ifdef USE_SOFTWARE_SERIAL
#include <SoftwareSerial.h>
SoftwareSerial COM_PORT(SOFTWARE_SERIAL_RX, SOFTWARE_SERIAL_TX, false);
#endif /* USE_SOFTWARE_SERIAL */

#ifdef USE_SOFTWARE_SERIAL
#define SERIAL_PARAM_5N1 SWSERIAL_5N1
#define SERIAL_PARAM_6N1 SWSERIAL_6N1
#define SERIAL_PARAM_7N1 SWSERIAL_7N1
#define SERIAL_PARAM_8N1 SWSERIAL_8N1
#define SERIAL_PARAM_5N2 SWSERIAL_5N2
#define SERIAL_PARAM_6N2 SWSERIAL_6N2
#define SERIAL_PARAM_7N2 SWSERIAL_7N2
#define SERIAL_PARAM_8N2 SWSERIAL_8N2
#define SERIAL_PARAM_5E1 SWSERIAL_5E1
#define SERIAL_PARAM_6E1 SWSERIAL_6E1
#define SERIAL_PARAM_7E1 SWSERIAL_7E1
#define SERIAL_PARAM_8E1 SWSERIAL_8E1
#define SERIAL_PARAM_5E2 SWSERIAL_5E2
#define SERIAL_PARAM_6E2 SWSERIAL_6E2
#define SERIAL_PARAM_7E2 SWSERIAL_7E2
#define SERIAL_PARAM_8E2 SWSERIAL_8E2
#define SERIAL_PARAM_5O1 SWSERIAL_5O1
#define SERIAL_PARAM_6O1 SWSERIAL_6O1
#define SERIAL_PARAM_7O1 SWSERIAL_7O1
#define SERIAL_PARAM_8O1 SWSERIAL_8O1
#define SERIAL_PARAM_5O2 SWSERIAL_5O2
#define SERIAL_PARAM_6O2 SWSERIAL_6O2
#define SERIAL_PARAM_7O2 SWSERIAL_7O2
#define SERIAL_PARAM_8O2 SWSERIAL_8O2
#else /* USE_SOFTWARE_SERIAL */
#define SERIAL_PARAM_5N1 SERIAL_5N1
#define SERIAL_PARAM_6N1 SERIAL_6N1
#define SERIAL_PARAM_7N1 SERIAL_7N1
#define SERIAL_PARAM_8N1 SERIAL_8N1
#define SERIAL_PARAM_5N2 SERIAL_5N2
#define SERIAL_PARAM_6N2 SERIAL_6N2
#define SERIAL_PARAM_7N2 SERIAL_7N2
#define SERIAL_PARAM_8N2 SERIAL_8N2
#define SERIAL_PARAM_5E1 SERIAL_5E1
#define SERIAL_PARAM_6E1 SERIAL_6E1
#define SERIAL_PARAM_7E1 SERIAL_7E1
#define SERIAL_PARAM_8E1 SERIAL_8E1
#define SERIAL_PARAM_5E2 SERIAL_5E2
#define SERIAL_PARAM_6E2 SERIAL_6E2
#define SERIAL_PARAM_7E2 SERIAL_7E2
#define SERIAL_PARAM_8E2 SERIAL_8E2
#define SERIAL_PARAM_5O1 SERIAL_5O1
#define SERIAL_PARAM_6O1 SERIAL_6O1
#define SERIAL_PARAM_7O1 SERIAL_7O1
#define SERIAL_PARAM_8O1 SERIAL_8O1
#define SERIAL_PARAM_5O2 SERIAL_5O2
#define SERIAL_PARAM_6O2 SERIAL_6O2
#define SERIAL_PARAM_7O2 SERIAL_7O2
#define SERIAL_PARAM_8O2 SERIAL_8O2
#endif /* USE_SOFTWARE_SERIAL */

/*
 * NICのライブラリのインクルード
 */
#ifdef USE_WIFI_NINA
#define USE_WIFI
#include <WiFiNINA.h>
#endif /* USE_WIFI_NINA */

#ifdef USE_NORMAL_WIFI
#define USE_WIFI
#include <WiFi.h>
#endif /* USE_NORMAL_WIFI */

#ifdef USE_NORMAL_ETHERNET
#define USE_ETHERNET
#include <Ethernet.h>
#endif /* USE_ETHERNET */

char passwd[][PASSWORD_LENGTH]={PASSWORD1, PASSWORD2, PASSWORD3};
uint8_t passwd_array_size=3;

#ifdef USE_DHCP
boolean useDhcp = true;   // DHCPでIPアドレスを設定
#else /* USE_DHCP */
boolean useDhcp = false;  // 固定IPアドレス
#endif /* USE_DHCP */

#ifdef USE_NORMAL_ETHERNET // ArduinoのイーサネットシールドはMACアドレスのROMがないので，定義が必要
byte mac[] = { 0x90, 0xa2, 0xda, 0x10, 0x11, 0x51 }; //アドレスは手持ちのarduinoのものに変更すること
#endif /* USE_NORMAL_ETHERNET */

IPAddress ip(192, 168, 1, 222);
IPAddress dnsServer(192, 168, 1, 1);
IPAddress gatewayAddress(192, 168, 1, 1);
IPAddress netMask(255, 255, 255, 0);

#ifdef USE_ETHERNET
EthernetServer server(PORT_NUMBER); // サーバオブジェクトの定義． 引数はポート番号
EthernetClient client;
#endif /* USE_ETHERNET */

#ifdef USE_WIFI
WiFiServer server(PORT_NUMBER); // サーバオブジェクトの定義． 引数はポート番号
WiFiClient client;
#endif /* USE_WIFI */

uint8_t state=STATE_START;
uint8_t passwd_retry=0;

uint32_t serialSpeed = DEFAULT_SERIAL_SPEED;

#if CPU_ARCH==AVR_ARCH /* AVR */
byte serialParameter = DEFAULT_SERIAL_PARAMETER;
#endif /* AVR */

#if CPU_ARCH==SAMD_ARCH /* MKR系列など */
byte serialParameter = DEFAULT_SERIAL_PARAMETER;
#endif /* MKR系列 */

#if CPU_ARCH==XTENSA_LX6_ARCH /* ESP32 */
uint32_t serialParameter = DEFAULT_SERIAL_PARAMETER;
#endif /* ESP32 */

#if CPU_ARCH==XTENSA_LX106_ARCH /* ESP8266 */
SoftwareSerialConfig serialParameter = DEFAULT_SERIAL_PARAMETER;
#endif /* ESP8266 */

HardwareHelper helper;

void logging(String str) {
#ifdef DEBUG
  Serial.print(str);
#endif /* DEBUG */
}

String DisplayAddress(IPAddress address) {
 return String(address[0]) + "." + 
        String(address[1]) + "." + 
        String(address[2]) + "." + 
        String(address[3]);
}

#ifdef USE_NORMAL_ETHERNET
void setupNetwork(void) {
  if (useDhcp) {
    if (Ethernet.begin(mac) == 0) {
      logging(F("DHCP fail.\n"));
      helper.SoftwareReset();
    }
  } else {
    Ethernet.begin(mac, ip, dnsServer, gatewayAddress, netMask);
    logging(F("ethernet setup done.\n"));
  }
  logging(F("IP address : ")); logging(DisplayAddress(Ethernet.localIP())); logging("\n");
}
#endif /* USE_NORMAL_ETHERNET */

#ifdef USE_WIFI
void setupNetwork(void) {
  if (useDhcp) {
    WiFi.begin(SSID_STR, WIFI_PASS);
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      logging( "." );
    }
    logging("\n");
  } else {
    WiFi.config(ip, dnsServer, gatewayAddress, netMask);
    WiFi.begin(SSID_STR, WIFI_PASS);
    while ( WiFi.status() != WL_CONNECTED ) {
      delay ( 500 );
      logging( "." );
    }
    logging("\n");
  }
  logging(F("IP address : ")); logging(DisplayAddress(WiFi.localIP())); logging("\n");
}
#endif /* USE_WIFI */

void setup() {
#ifdef DEBUG
  Serial.begin(DEBUG_SERIAL_SPEED) ;    // シリアル通信の初期化
  while (!Serial) {       // シリアルポートが開くのを待つ
    ;
  }
#endif /* DEBUG */
  logging("setup start.\n");
#ifdef USE_HARD_RESET_PIN
  pinMode(USE_HARD_RESET_PIN, OUTPUT);
  digitalWrite(USE_HARD_RESET_PIN, LOW);
#endif /* USE_HARD_RESET_PIN */
  setupNetwork();
  logging(F("network setup done.\n"));
  state=STATE_WAIT_CLIENT;
  server.begin();
  COM_PORT.begin(serialSpeed, serialParameter);
}

uint8_t WaitClient(void) {
  client = server.available();
  if (client) {
    logging(F("Connection established by a client\n"));
    client.write(0xff);client.write(0xfd);client.write(0x03); // IAC DO SGA   クライアントにlineモードを使わせない
    client.write(0xff);client.write(0xfb);client.write(0x01); // IAC DO ECHO
    logging("\nstart telnet negotiation.\n");
    delay(1000);
    int num;
    if (num=client.available()) {
      for (int i=0; i< num ; i++) {
        char c = client.read();
#ifdef DEBUG
        char buffer[3]; sprintf (buffer, "%02x", c);
        Serial.print(buffer);
#endif /* DEBUG */
      }
    }
    logging("\nend telnet negotiation.\n");
    /* telnet negotiation */
    return STATE_INPUT_PASSWD;
  }
  return STATE_WAIT_CLIENT;
}

bool CheckPasswd(char * buff){
  String input=String(buff);
  for (int i=0; i< passwd_array_size ; i++) {
    String s=String(passwd[i]);
    if (0==input.compareTo(s)) return true;
  }
  return false;
}

uint8_t InputPassword(void) {
  unsigned long lastTime=millis();
  uint8_t index=0;
  char buff[PASSWORD_LENGTH]="";
  client.print("login password : ");
  while (client.connected()) {
    if (client.available()) {
      char c = client.read(); //一文字読み取り
#ifdef DEBUG
      char buffer[3]; sprintf (buffer, "%02x", c);
      Serial.print(buffer);
#endif /* DEBUG */
      if ((c==LF)||(c==CR)) {
        client.println("");
#ifdef DEBUG
        Serial.println("");
        Serial.print("passwd = { ");
        for (int i=0 ; i< PASSWORD_LENGTH; i++) {
          Serial.print(buff[i], HEX);
          Serial.print(" ");
        }
        Serial.println(" }");
#endif /* DEBUG */
        if (CheckPasswd(buff)) {
          logging("login success.\n");
          client.println("network serial server.");
          client.println("Please input command.");
          PrintHelp();
          return STATE_OPERATION;
        } else {
          passwd_retry++;
#ifdef DEBUG
          Serial.print("retry number =");
          Serial.println(passwd_retry);
#endif /* DEBUG */
          if (passwd_retry==PASSWD_RETRY_MAX) {
            client.println("login failure.");
            client.stop();
            logging("\n");
            logging("login failure.\n");
            logging("connection closed.\n");
            return STATE_WAIT_CLIENT;
          }
          return STATE_INPUT_PASSWD;
        }
      } else if ( (PASSWD_CHAR_START <= c ) && (PASSWD_CHAR_END >= c)) {
#ifdef DEBUG
        Serial.print(c);
#endif /* DEBUG */
        buff[index]=c;
        index++;
        client.print("*");
      }
      lastTime=millis();
    } else {
      unsigned long diff=millis() - lastTime;
      if (INPUT_TIMEOUT < diff) {
        client.stop();
        logging("\n");
        logging("command input timeout");
        logging("connection closed.\n");
        return STATE_WAIT_CLIENT;
      }
    }
  }
  client.stop();
  logging("\n");
  logging("connection closed.\n");
  return STATE_WAIT_CLIENT;
}

void PrintPrompt(void) {
  client.print("CM> ");
}

void PrintHelp(void) {
  client.println("");
  client.println("Commands are :");
  client.println(" 's' or 'S'        - serial speed select menu.");
  client.println(" 'o' or 'O'        - open serial port.");
  client.println(" 'r' or 'R'        - reboot.");
  client.println(" 'q' or 'Q'        - terminate telnet connection.");
#ifdef USE_HARD_RESET_PIN
  client.println(" 'x' or 'X'        - hard reset(target system).");
#endif /* USE_HARD_RESET_PIN */
  client.println(" 'h' or 'H' or '?' - print this help.");
}

bool SelectSerialSpeed(void){
  unsigned long lastTime=millis();
RESTART:
  client.println("");
  client.println("Serial port setting menu.");
  client.println("Select serial port speed :");
  client.println(" a -     300bps");
  client.println(" b -    1200bps");
  client.println(" c -    2400bps");
  client.println(" d -    4800bps");
  client.println(" e -    9600bps");
  client.println(" f -   19200bps");
  client.println(" g -   38400bps");
  client.println(" h -   57600bps");
  client.println(" i -   74800bps");
  client.println(" j -  115200bps");
  client.println(" k -  230400bps");
  client.println(" l -  250000bps");
  client.println(" m -  500000bps");
  client.println(" n - 1000000bps");
  client.println(" o - 2000000bps");
  PrintPrompt();
  while (client.connected()) {
    unsigned long diff=millis() - lastTime;
    if (INPUT_TIMEOUT < diff) {
      client.stop();
      logging("\n");
      logging("command input timeout");
      logging("connection closed.\n");
      return false;
    }
    if (client.available()) {
      char c = client.read(); //一文字読み取り
      if ( (PASSWD_CHAR_START <= c ) && (PASSWD_CHAR_END >= c)) client.println(c);
      lastTime=millis();
      switch(c) {
        case 'a':serialSpeed=300;break;
        case 'b':serialSpeed=1200;break;
        case 'c':serialSpeed=2400;break;
        case 'd':serialSpeed=4800;break;
        case 'e':serialSpeed=9600;break;
        case 'f':serialSpeed=19200;break;
        case 'g':serialSpeed=38400;break;
        case 'h':serialSpeed=57600;break;
        case 'i':serialSpeed=74800;break;
        case 'j':serialSpeed=115200;break;
        case 'k':serialSpeed=230400;break;
        case 'l':serialSpeed=250000;break;
        case 'm':serialSpeed=500000;break;
        case 'n':serialSpeed=1000000;break;
        case 'o':serialSpeed=2000000;break;
        default:
          client.println("error : invalid input.");goto RESTART;
      }
      return true;
    }
  }
  logging("connection closed.\n");
  return false;
}


bool SelectSerialParameter(void){
  unsigned long lastTime=millis();
RESTART:
  client.println("");
  client.println("Serial parameter selection :");
  client.println(" a - 5N1 5bit data, non parity, stop bit=1");
  client.println(" b - 6N1");
  client.println(" c - 7N1");
  client.println(" d - 8N1");
  client.println(" e - 5N2 5bit data, non parity, stop bit=2");
  client.println(" f - 6N2");
  client.println(" g - 7N2");
  client.println(" h - 8N2");
  client.println(" i - 5E1 5bit data, even parity, stop bit=1");
  client.println(" j - 6E1");
  client.println(" k - 7E1");
  client.println(" l - 8E1");
  client.println(" m - 5E2 5bit data, even parity, stop bit=2");
  client.println(" n - 6E2");
  client.println(" o - 7E2");
  client.println(" p - 8E2");
  client.println(" q - 5O1 5bit data, odd parity, stop bit=1");
  client.println(" r - 6O1");
  client.println(" s - 7O1");
  client.println(" t - 8O1");
  client.println(" u - 5O2 5bit data, odd parity, stop bit=2");
  client.println(" v - 6O2");
  client.println(" w - 7O2");
  client.println(" x - 8O2");
  PrintPrompt();
  while (client.connected()) {
    unsigned long diff=millis() - lastTime;
    if (INPUT_TIMEOUT < diff) {
      client.stop();
      logging("\n");
      logging("command input timeout");
      logging("connection closed.\n");
      return false;
    }
    if (client.available()) {
      char c = client.read(); //一文字読み取り
      if ( (PASSWD_CHAR_START <= c ) && (PASSWD_CHAR_END >= c)) client.println(c);
      lastTime=millis();
      switch(c) {
        case 'a':serialParameter=SERIAL_PARAM_5N1;break;
        case 'b':serialParameter=SERIAL_PARAM_6N1;break;
        case 'c':serialParameter=SERIAL_PARAM_7N1;break;
        case 'd':serialParameter=SERIAL_PARAM_8N1;break;
        case 'e':serialParameter=SERIAL_PARAM_5N2;break;
        case 'f':serialParameter=SERIAL_PARAM_6N2;break;
        case 'g':serialParameter=SERIAL_PARAM_7N2;break;
        case 'h':serialParameter=SERIAL_PARAM_8N2;break;
        case 'i':serialParameter=SERIAL_PARAM_5E1;break;
        case 'j':serialParameter=SERIAL_PARAM_6E1;break;
        case 'k':serialParameter=SERIAL_PARAM_7E1;break;
        case 'l':serialParameter=SERIAL_PARAM_8E1;break;
        case 'm':serialParameter=SERIAL_PARAM_5E2;break;
        case 'n':serialParameter=SERIAL_PARAM_6E2;break;
        case 'o':serialParameter=SERIAL_PARAM_7E2;break;
        case 'p':serialParameter=SERIAL_PARAM_8E2;break;
        case 'q':serialParameter=SERIAL_PARAM_5O1;break;
        case 'r':serialParameter=SERIAL_PARAM_6O1;break;
        case 's':serialParameter=SERIAL_PARAM_7O1;break;
        case 't':serialParameter=SERIAL_PARAM_8O1;break;
        case 'u':serialParameter=SERIAL_PARAM_5O2;break;
        case 'v':serialParameter=SERIAL_PARAM_6O2;break;
        case 'w':serialParameter=SERIAL_PARAM_7O2;break;
        case 'x':serialParameter=SERIAL_PARAM_8O2;break;
        default: client.println("error : invalid input.");goto RESTART;
      }
      return true;
    }
  }
  logging("connection closed.\n");
  return false;
}

uint8_t SerialSetting(void){
  if (!SelectSerialSpeed()) return STATE_WAIT_CLIENT;
  if (!SelectSerialParameter()) return STATE_WAIT_CLIENT;
  COM_PORT.begin(serialSpeed, serialParameter);
  return STATE_OPERATION;
}

void PrintCtrlCode(void) {
  switch(CTRL_CODE) {
    case 0x11:client.println("Escape from communication code = Ctrl-q");break;
    case 0x12:client.println("Escape from communication code = Ctrl-r");break;
    case 0x13:client.println("Escape from communication code = Ctrl-s");break;
    case 0x14:client.println("Escape from communication code = Ctrl-t");break;
  }
}

#ifdef USE_HARD_RESET_PIN
void ExecHardReset(void) {
  digitalWrite(USE_HARD_RESET_PIN, HIGH);
  delay(HARD_RESET_TIME);
  digitalWrite(USE_HARD_RESET_PIN, LOW);
}
#endif /* USE_HARD_RESET_PIN */

uint8_t CommandInput(void) {
  unsigned long lastTime=millis();
  int garbase;
  if (garbase=client.available()) {
    for (int i=0; i< garbase ; i++) {
      char c = client.read();
    }
  }
  PrintPrompt();
  while (client.connected()) {
    if (client.available()) {
      char c = client.read(); //一文字読み取り
      if ( (PASSWD_CHAR_START <= c ) && (PASSWD_CHAR_END >= c)) client.println(c);
      lastTime=millis();
      switch(c) {
        case 'o':
        case 'O':
          PrintCtrlCode();
          return STATE_COMMUNICATION;
        case 's':
        case 'S':
          return SerialSetting();
        case 'q':
        case 'Q':
          client.stop();
          logging("\n");
          logging("connection closed.\n");
          return STATE_WAIT_CLIENT;
        case 'r':
        case 'R':
          client.stop();
          helper.SoftwareReset();
#ifdef USE_HARD_RESET_PIN
        case 'x':
        case 'X':
          ExecHardReset();
          return STATE_OPERATION;
#endif /* USE_HARD_RESET_PIN */
        case 'h':
        case 'H':
        case '?':
          PrintHelp();PrintPrompt();break;
        default:
          client.println("error : invalid command");PrintPrompt();
      }
    } else {
      unsigned long diff=millis() - lastTime;
      if (INPUT_TIMEOUT < diff) {
        client.stop();
        logging("\n");
        logging("command input timeout");
        logging("connection closed.\n");
        return STATE_WAIT_CLIENT;
      }
    }
  }
  client.stop();
  logging("\n");
  logging("connection closed.\n");
  return STATE_WAIT_CLIENT;
}

uint8_t SerialCommunication(void){
  unsigned long lastTime=millis();
  while (client.connected()) {
    int num;
    if (num=client.available()) {
      for (int i=0; i< num ; i++) {
        char c = client.read(); //一文字読み取り
        if (CTRL_CODE==c) return STATE_OPERATION;
        COM_PORT.print(c);
      }
      lastTime=millis();
    } else {
      unsigned long diff=millis() - lastTime;
      if (INPUT_TIMEOUT < diff) {
        client.stop();
        logging("\n");
        logging("command input timeout");
        logging("connection closed.\n");
        return STATE_WAIT_CLIENT;
      }
      int cnum;
      if (cnum=COM_PORT.available()) {
        for (int i=0; i< cnum ; i++) {
          char c = COM_PORT.read();
          client.print(c);
        }
      }
    }
  }
  return STATE_WAIT_CLIENT; 
}

void loop() {
  switch(state) {
    case STATE_WAIT_CLIENT:  state=WaitClient(); passwd_retry=0; break;
    case STATE_INPUT_PASSWD: state=InputPassword(); break;
    case STATE_OPERATION: state=CommandInput(); break;
    case STATE_COMMUNICATION: state=SerialCommunication();break;
  }
}

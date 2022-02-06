#define DEBUG
#define DEBUG_SERIAL_SPEED 9600

/*
 * ネットワーク関係の定義
 */
//#define USE_ETHERNET_W5XXX
//#define USE_ETHERNET
//#define USE_WIFI_NINA
//#define USE_WIFI_NORMAL
//#define USE_WIFI

//#define USE_DHCP                 // IPアドレスをDHCPで行う (コメントアウトした場合は固定IP)

/*
 * WiFi関係
 */
#define WIFI_SSID "foo"
#define WIFI_PASS "bar"

/*
 * シリアル関係
 */
//#define USE_CUSTOM_SERIAL

/*
 * シリアル通信用I/Fのデフォルトパラメータの定義(デバッグ用ではない)
 */
#define DEFAULT_SERIAL_SPEED 115200
#define DEFAULT_SERIAL_PARAMETER SERIAL_8N1
//#define DEFAULT_SERIAL_PARAMETER SWSERIAL_8N1

/*
 * Raspberry Pi関係
 */
#define USE_HARD_RESET_PIN 15 // Espr one 32のD2 これを定義すると、ハードリセットプログラムがコンパイルされる
#define HARD_RESET_TIME 1000  // 1秒(定義の単位はms)

/*
 * telnetのポート番号定義
 */
#define PORT_NUMBER 23

/*
 * パスワード関係
 */
#define PASSWORD_LENGTH 16
#define PASSWORD1 "hoge"
#define PASSWORD2 "foo"
#define PASSWORD3 "bar"
#define PASSWD_NUM 3

/*
 * telnetでのパスワード認証をPASSWD_RETRY_MAX回失敗するとconnectionは切る
 */
#define PASSWD_RETRY_MAX  3

/*
 * 一定時間入力がない場合に，通信を切断するので，その時間をミリ秒単位で定義
 */
#define INPUT_TIMEOUT 300000

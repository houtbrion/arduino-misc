
#include <RestServer.h>
#include <IRremote.hpp>

#include <detectArduinoHardware.h>

/* ESP32はIR_SEND_PINが規定されているがそれ以外は自分でピンを決める */
#ifndef IR_SEND_PIN
#define IR_LED_PIN 4
#endif /* IR_SEND_PIN */

#define USE_DHCP

#define WIFI_SSID "foo"
#define WIFI_PASS "bar"

#ifndef USE_DHCP
IPAddress ip(192, 168, 1, 222);
IPAddress dnsServer(192, 168, 1, 1);
IPAddress gatewayAddress(192, 168, 1, 1);
IPAddress netMask(255, 255, 255, 0);
#endif /* USE_DHCP */


#ifdef USE_WIFI
WiFiServer server(80);
#else /* USE_WIFI */
EthernetServer server(80);
#endif /* USE_WIFI */
RestServer rest(server); // You need to pass the EthernetServer reference to the RestServer




#define MAX_NUM_PARAMS 5


int split(String data, char delimiter, String *dst){
  int index = 0;
  int arraySize = (sizeof(data))/sizeof((data[0]));
  int datalength = data.length();
  
  for(int i = 0; i < datalength; i++){
    char tmp = data.charAt(i);
    if( tmp == delimiter ){
      index++;
      if( index > (arraySize - 1)) return -1;
    }
    else dst[index] += tmp;
  }
  return (index + 1);
}

typedef enum {
  ARG_TYPE_21=(21),
  ARG_TYPE_22,
  ARG_TYPE_23,
  ARG_TYPE_24,
  ARG_TYPE_25,
  ARG_TYPE_26,
  ARG_TYPE_27,
  ARG_TYPE_28,
  ARG_TYPE_31=(31),
  ARG_TYPE_32,
  ARG_TYPE_33,
  ARG_TYPE_34,
  ARG_TYPE_35,
  ARG_TYPE_36,
  ARG_TYPE_37,
  ARG_TYPE_38,
  ARG_TYPE_41=(41),
  ARG_TYPE_42,
  ARG_TYPE_43,
  ARG_TYPE_44,
  ARG_TYPE_45,
  ARG_TYPE_46,
  ARG_TYPE_51=(51)
} argType_t;

typedef struct {
  unsigned int arg1;
  int arg2;
} argsType21_t;

typedef struct {
  uint32_t arg1;
  uint16_t arg2;
} argsType22_t;

typedef struct {
  uint32_t arg1;
  uint8_t arg2;
} argsType23_t;

typedef struct {
  unsigned int arg1;
  unsigned int arg2;
} argsType24_t;

typedef struct {
  unsigned long arg1;
  int arg2;
} argsType25_t;

typedef struct {
  uint8_t arg1;
  uint_fast8_t arg2;
} argsType26_t;

typedef struct {
  uint16_t arg1;
  bool arg2;
} argsType27_t;

typedef struct {
  uint16_t arg1;
  uint32_t arg2;
} argsType28_t;

typedef struct {
  uint8_t arg1;
  uint8_t arg2;
  uint_fast8_t arg3;
} argsType31_t;

typedef struct {
  uint16_t arg1;
  uint8_t arg2;
  uint_fast8_t arg3;
} argsType32_t;

typedef struct {
  uint16_t arg1;
  unsigned int arg2;
  uint_fast8_t arg3;
} argsType33_t;

typedef struct {
  unsigned long arg1;
  int arg2;
  bool arg3;
} argsType34_t;

typedef struct {
  uint16_t arg1;
  uint8_t arg2;
  bool arg3;
} argsType35_t;

typedef struct {
  uint32_t arg1;
  uint8_t arg2;
  bool arg3;
} argsType36_t;

typedef struct {
  uint8_t arg1;
  uint8_t arg2;
  bool arg3;
} argsType37_t;

typedef struct {
  unsigned int arg1;
  uint32_t arg2;
  uint_fast8_t arg3;
} argsType38_t;

typedef struct {
  uint16_t arg1;
  uint8_t arg2;
  uint_fast8_t arg3;
  uint16_t arg4;
} argsType41_t;

typedef struct {
  uint16_t arg1;
  uint8_t arg2;
  uint_fast8_t arg3;
  uint8_t arg4;
} argsType42_t;

typedef struct {
  uint8_t arg1;
  uint8_t arg2;
  uint_fast8_t arg3;
  bool arg4;
} argsType43_t;

typedef struct {
  uint8_t arg1;
  uint8_t arg2;
  uint8_t arg3;
  bool arg4;
} argsType44_t;

typedef struct {
  uint16_t arg1;
  uint8_t arg2;
  uint_fast8_t arg3;
  bool arg4;
} argsType45_t;

typedef struct {
  uint16_t arg1;
  uint16_t arg2;
  uint_fast8_t arg3;
  bool arg4;
} argsType46_t;

typedef struct {
  uint8_t arg1;
  uint16_t arg2;
  uint_fast8_t arg3;
  bool arg4;
  bool arg5;
} argsType51_t;



typedef union argsType_t {
  argsType21_t  argsType21;
  argsType22_t  argsType22;
  argsType23_t  argsType23;
  argsType24_t  argsType24;
  argsType25_t  argsType25;
  argsType26_t  argsType26;
  argsType27_t  argsType27;
  argsType28_t  argsType28;
  argsType31_t  argsType31;
  argsType32_t  argsType32;
  argsType33_t  argsType33;
  argsType34_t  argsType34;
  argsType35_t  argsType35;
  argsType36_t  argsType36;
  argsType37_t  argsType37;
  argsType38_t  argsType38;
  argsType41_t  argsType41;
  argsType42_t  argsType42;
  argsType43_t  argsType43;
  argsType44_t  argsType44;
  argsType45_t  argsType45;
  argsType46_t  argsType46;
  argsType51_t  argsType51;
};

bool analyzeParam(String param, uint8_t argType, union argsType_t *args) {
  String paramArray[MAX_NUM_PARAMS];
  int num=split(param, '&' , paramArray);
  int argNum;
  switch(argType) {
    case ARG_TYPE_21: argNum=2;break;
    case ARG_TYPE_22: argNum=2;break;
    case ARG_TYPE_23: argNum=2;break;
    case ARG_TYPE_24: argNum=2;break;
    case ARG_TYPE_25: argNum=2;break;
    case ARG_TYPE_26: argNum=2;break;
    case ARG_TYPE_27: argNum=2;break;
    case ARG_TYPE_28: argNum=2;break;
    case ARG_TYPE_31: argNum=3;break;
    case ARG_TYPE_32: argNum=3;break;
    case ARG_TYPE_33: argNum=3;break;
    case ARG_TYPE_34: argNum=3;break;
    case ARG_TYPE_35: argNum=3;break;
    case ARG_TYPE_36: argNum=3;break;
    case ARG_TYPE_37: argNum=3;break;
    case ARG_TYPE_41: argNum=4;break;
    case ARG_TYPE_42: argNum=4;break;
    case ARG_TYPE_43: argNum=4;break;
    case ARG_TYPE_44: argNum=4;break;
    case ARG_TYPE_45: argNum=4;break;
    case ARG_TYPE_46: argNum=4;break;
    case ARG_TYPE_51: argNum=5;break;
    default: return false;
  }
  if (num != argNum) return false;

  switch(argType) {
    case ARG_TYPE_21:
      args->argsType21.arg1=(unsigned int)paramArray[0].toInt();
      args->argsType21.arg2=(int)paramArray[1].toInt();
      break;
    case ARG_TYPE_22:
      args->argsType22.arg1=(uint32_t)paramArray[0].toInt();
      args->argsType22.arg2=(uint16_t)paramArray[1].toInt();
      break;
    case ARG_TYPE_23:
      args->argsType23.arg1=(uint32_t)paramArray[0].toInt();
      args->argsType23.arg2=(uint8_t)paramArray[1].toInt();
      break;
    case ARG_TYPE_24:
      args->argsType24.arg1=(unsigned int)paramArray[0].toInt();
      args->argsType24.arg2=(unsigned int)paramArray[1].toInt();
      break;
    case ARG_TYPE_25:
      args->argsType25.arg1=(unsigned long)paramArray[0].toInt();
      args->argsType25.arg2=(int)paramArray[1].toInt();
      break;
    case ARG_TYPE_26:
      args->argsType26.arg1=(uint8_t)paramArray[0].toInt();
      args->argsType26.arg2=(uint_fast8_t)paramArray[1].toInt();
      break;
    case ARG_TYPE_27:
      args->argsType27.arg1=(uint16_t)paramArray[0].toInt();
      if (0==paramArray[1].toInt()) {
        args->argsType27.arg2=false;
      } else {
        args->argsType27.arg2=true;
      }
      break;
    case ARG_TYPE_28:
      args->argsType28.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType28.arg2=(uint32_t)paramArray[1].toInt();
      break;
    case ARG_TYPE_31:
      args->argsType31.arg1=(uint8_t)paramArray[0].toInt();
      args->argsType31.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType31.arg3=(uint_fast8_t)paramArray[2].toInt();
      break;
    case ARG_TYPE_32:
      args->argsType32.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType32.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType32.arg3=(uint_fast8_t)paramArray[2].toInt();
      break;
    case ARG_TYPE_33:
      args->argsType33.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType33.arg2=(unsigned int)paramArray[1].toInt();
      args->argsType33.arg3=(uint_fast8_t)paramArray[2].toInt();
      break;
    case ARG_TYPE_34:
      args->argsType34.arg1=(unsigned long)paramArray[0].toInt();
      args->argsType34.arg2=(int)paramArray[1].toInt();
      if (0==paramArray[2].toInt()) {
        args->argsType34.arg3=false;
      } else {
        args->argsType34.arg3=true;
      }
      break;
    case ARG_TYPE_35:
      args->argsType35.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType35.arg2=(uint8_t)paramArray[1].toInt();
      if (0==paramArray[2].toInt()) {
        args->argsType35.arg3=false;
      } else {
        args->argsType35.arg3=true;
      }
      break;
    case ARG_TYPE_36:
      args->argsType36.arg1=(uint32_t)paramArray[0].toInt();
      args->argsType36.arg2=(uint8_t)paramArray[1].toInt();
      if (0==paramArray[2].toInt()) {
        args->argsType36.arg3=false;
      } else {
        args->argsType36.arg3=true;
      }
      break;
    case ARG_TYPE_37:
      args->argsType37.arg1=(uint8_t)paramArray[0].toInt();
      args->argsType37.arg2=(uint8_t)paramArray[1].toInt();
      if (0==paramArray[2].toInt()) {
        args->argsType37.arg3=false;
      } else {
        args->argsType37.arg3=true;
      }
      break;
    case ARG_TYPE_38:
      args->argsType38.arg1=(unsigned int)paramArray[0].toInt();
      args->argsType38.arg2=(uint32_t)paramArray[1].toInt();
      args->argsType38.arg3=(uint_fast8_t)paramArray[2].toInt();
      break;
    case ARG_TYPE_41:
      args->argsType41.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType41.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType41.arg3=(uint_fast8_t)paramArray[2].toInt();
      args->argsType41.arg4=(uint16_t)paramArray[3].toInt();
      break;
    case ARG_TYPE_42:
      args->argsType42.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType42.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType42.arg3=(uint_fast8_t)paramArray[2].toInt();
      args->argsType42.arg4=(uint8_t)paramArray[3].toInt();
      break;
    case ARG_TYPE_43:
      args->argsType43.arg1=(uint8_t)paramArray[0].toInt();
      args->argsType43.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType43.arg3=(uint_fast8_t)paramArray[2].toInt();
      if (0==paramArray[3].toInt()) {
        args->argsType43.arg4=false;
      } else {
        args->argsType43.arg4=true;
      }
      break;
    case ARG_TYPE_44:
      args->argsType44.arg1=(uint8_t)paramArray[0].toInt();
      args->argsType44.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType44.arg3=(uint8_t)paramArray[2].toInt();
      if (0==paramArray[3].toInt()) {
        args->argsType44.arg4=false;
      } else {
        args->argsType44.arg4=true;
      }
      break;
    case ARG_TYPE_45:
      args->argsType45.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType45.arg2=(uint8_t)paramArray[1].toInt();
      args->argsType45.arg3=(uint_fast8_t)paramArray[2].toInt();
      if (0==paramArray[3].toInt()) {
        args->argsType45.arg4=false;
      } else {
        args->argsType45.arg4=true;
      }
      break;
    case ARG_TYPE_46:
      args->argsType46.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType46.arg2=(uint16_t)paramArray[1].toInt();
      args->argsType46.arg3=(uint_fast8_t)paramArray[2].toInt();
      if (0==paramArray[3].toInt()) {
        args->argsType46.arg4=false;
      } else {
        args->argsType46.arg4=true;
      }
      break;
    case ARG_TYPE_51:
      args->argsType51.arg1=(uint16_t)paramArray[0].toInt();
      args->argsType51.arg2=(uint16_t)paramArray[1].toInt();
      args->argsType51.arg3=(uint_fast8_t)paramArray[2].toInt();
      if (0==paramArray[3].toInt()) {
        args->argsType51.arg4=false;
      } else {
        args->argsType51.arg4=true;
      }
      if (0==paramArray[4].toInt()) {
        args->argsType51.arg5=false;
      } else {
        args->argsType51.arg5=true;
      }
      break;
    default: return false;
  }

  return true;
}

void necSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_45, &args)) return;
  IrSender.sendNEC(args.argsType45.arg1,args.argsType45.arg2,args.argsType45.arg3,args.argsType45.arg4);
}

void lgSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_51, &args)) return;
  IrSender.sendLG(args.argsType51.arg1,args.argsType51.arg2,args.argsType51.arg3,args.argsType51.arg4,args.argsType51.arg5);
}

void kaseikyoSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_41, &args)) return;
  IrSender.sendKaseikyo(args.argsType41.arg1,args.argsType41.arg2,args.argsType41.arg3,args.argsType41.arg4);
}

void sonySend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_42, &args)) return;
  IrSender.sendSony(args.argsType42.arg1,args.argsType42.arg2,args.argsType42.arg3,args.argsType42.arg4);
}

void appleSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_43, &args)) return;
  IrSender.sendApple(args.argsType43.arg1,args.argsType43.arg2,args.argsType43.arg3,args.argsType43.arg4);
}

void denonSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_43, &args)) return;
  IrSender.sendDenon(args.argsType43.arg1,args.argsType43.arg2,args.argsType43.arg3,args.argsType43.arg4);
}

void rc5Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_43, &args)) return;
  IrSender.sendRC5(args.argsType43.arg1,args.argsType43.arg2,args.argsType43.arg3,args.argsType43.arg4);
}

void rc6Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_43, &args)) return;
  IrSender.sendRC6(args.argsType43.arg1,args.argsType43.arg2,args.argsType43.arg3,args.argsType43.arg4);
}

void legoPowerFunctionSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_44, &args)) return;
  IrSender.sendLegoPowerFunctions(args.argsType44.arg1,args.argsType44.arg2,args.argsType44.arg3,args.argsType44.arg4);
}

void onkyoSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_46, &args)) return;
  IrSender.sendOnkyo(args.argsType46.arg1,args.argsType46.arg2,args.argsType46.arg3,args.argsType46.arg4);
}

void samsungSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_46, &args)) return;
  IrSender.sendSamsung(args.argsType46.arg1,args.argsType46.arg2,args.argsType46.arg3,args.argsType46.arg4);
}

void sharpSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_31, &args)) return;
  IrSender.sendSharp(args.argsType31.arg1,args.argsType31.arg2,args.argsType31.arg3);
}

void kaseikyo_denonSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_32, &args)) return;
  IrSender.sendKaseikyo_Denon(args.argsType32.arg1,args.argsType32.arg2,args.argsType32.arg3);
}

void kaseikyo_jvcSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_32, &args)) return;
  IrSender.sendKaseikyo_JVC(args.argsType32.arg1,args.argsType32.arg2,args.argsType32.arg3);
}

void kaseikyo_mitsubishiSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_32, &args)) return;
  IrSender.sendKaseikyo_Mitsubishi(args.argsType32.arg1,args.argsType32.arg2,args.argsType32.arg3);
}

void kaseikyo_sharpSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_32, &args)) return;
  IrSender.sendKaseikyo_Sharp(args.argsType32.arg1,args.argsType32.arg2,args.argsType32.arg3);
}

void panasonicSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_32, &args)) return;
  IrSender.sendPanasonic(args.argsType32.arg1,args.argsType32.arg2,args.argsType32.arg3);
}

void jvcSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_34, &args)) return;
  IrSender.sendJVC(args.argsType34.arg1,args.argsType34.arg2,args.argsType34.arg3);
}

void jvcMSBSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_34, &args)) return;
  IrSender.sendJVCMSB(args.argsType34.arg1,args.argsType34.arg2,args.argsType34.arg3);
}

void legoPowerFunction3Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_35, &args)) return;
  IrSender.sendLegoPowerFunctions(args.argsType35.arg1,args.argsType35.arg2,args.argsType35.arg3);
}

void necMSBSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_36, &args)) return;
  IrSender.sendNECMSB(args.argsType36.arg1,args.argsType36.arg2,args.argsType36.arg3);
}

void rc5extSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_37, &args)) return;
  IrSender.sendRC5ext(args.argsType37.arg1,args.argsType37.arg2,args.argsType37.arg3);
}

void biPhaseSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_38, &args)) return;
  IrSender.sendBiphaseData(args.argsType38.arg1,args.argsType38.arg2,args.argsType38.arg3);
}

void denon2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_21, &args)) return;
  IrSender.sendDenon(args.argsType21.arg1,args.argsType21.arg2);
}

void dishSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_21, &args)) return;
  IrSender.sendDISH(args.argsType21.arg1,args.argsType21.arg2);
}

void lg2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_21, &args)) return;
  IrSender.sendLG(args.argsType21.arg1,args.argsType21.arg2);
}

void samsung2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_21, &args)) return;
  IrSender.sendSAMSUNG(args.argsType21.arg1,args.argsType21.arg2);
}

void sony2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_21, &args)) return;
  IrSender.sendSony(args.argsType21.arg1,args.argsType21.arg2);
}

void magiQuestSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_22, &args)) return;
  IrSender.sendMagiQuest(args.argsType22.arg1,args.argsType22.arg2);
}

void rc5_2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_23, &args)) return;
  IrSender.sendRC5(args.argsType23.arg1,args.argsType23.arg2);
}

void rc6_2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_23, &args)) return;
  IrSender.sendRC6(args.argsType23.arg1,args.argsType23.arg2);
}

void nec2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_23, &args)) return;
  IrSender.sendNEC(args.argsType23.arg1,args.argsType23.arg2);
}

void sharp2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_24, &args)) return;
  IrSender.sendSharp(args.argsType24.arg1,args.argsType24.arg2);
}

void whynterSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_25, &args)) return;
  IrSender.sendWhynter(args.argsType25.arg1,args.argsType25.arg2);
}

void bosewaveSend(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_26, &args)) return;
  IrSender.sendBoseWave(args.argsType26.arg1,args.argsType26.arg2);
}

void legoPowerFunction2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_27, &args)) return;
  IrSender.sendLegoPowerFunctions(args.argsType27.arg1,args.argsType27.arg2);
}

void panasonic2Send(char * params = "") {
  String paramString=String(params);
  union argsType_t args;
  if (false==analyzeParam(paramString, ARG_TYPE_28, &args)) return;
  IrSender.sendPanasonic(args.argsType28.arg1,args.argsType28.arg2);
}

/* *************************************************************** */

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600) ;    // シリアル通信の初期化
  while (!Serial) {       // シリアルポートが開くのを待つ
    ;
  }
  Serial.println("Init...");
  
  // Start Ethernet lib
  
#ifdef USE_WIFI
#ifndef USE_DHCP
  WiFi.config(ip, dnsServer, gatewayAddress, netMask);
#endif /* USE_DHCP */
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print( "." );
  }
  Serial.println("");
#else /* USE_WIFI */
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
#ifdef USE_DHCP
  Ethernet.begin(mac);
#else /* USE_DHCP */
  Ethernet.begin(mac, ip, dnsServer, gatewayAddress, netMask);
#endif /* USE_DHCP */
#endif /* USE_WIFI */
  
  // // Start Ethernet server
  server.begin();
  Serial.println("Server is at ");
#ifdef USE_WIFI
  Serial.println( WiFi.localIP() );
#else /* USE_WIFI */
  Serial.println( Ethernet.localIP() );
#endif /* USE_WIFI */

  // Add routes to the REST Server
  rest.addRoute(GET, "/nec", necSend);
#ifndef MINIMUM
  rest.addRoute(GET, "/lg", lgSend);
  rest.addRoute(GET, "/kaseikyo", kaseikyoSend);
  rest.addRoute(GET, "/sony", sonySend);
  rest.addRoute(GET, "/apple", appleSend);
  rest.addRoute(GET, "/denon", denonSend);
  rest.addRoute(GET, "/rc5", rc5Send);
  rest.addRoute(GET, "/rc6", rc6Send);
  rest.addRoute(GET, "/legoPowerFunction", legoPowerFunctionSend);
  rest.addRoute(GET, "/nec2", nec2Send);
  rest.addRoute(GET, "/onkyo", onkyoSend);
  rest.addRoute(GET, "/samsung", samsungSend);
  rest.addRoute(GET, "/sharp", sharpSend);
  rest.addRoute(GET, "/kaseikyo_denon", kaseikyo_denonSend);
  rest.addRoute(GET, "/kaseikyo_jvc", kaseikyo_jvcSend);
  rest.addRoute(GET, "/kaseikyo_mitsubishi", kaseikyo_mitsubishiSend);
  rest.addRoute(GET, "/kaseikyo_sharp", kaseikyo_sharpSend);
  rest.addRoute(GET, "/panasonic", panasonicSend);
  rest.addRoute(GET, "/jvc", jvcSend);
  rest.addRoute(GET, "/jvcMSB", jvcMSBSend);
  rest.addRoute(GET, "/legoPowerFunction3", legoPowerFunction3Send);
  rest.addRoute(GET, "/necMSB", necMSBSend);
  rest.addRoute(GET, "/rc5ext", rc5extSend);
  rest.addRoute(GET, "/denon2", denon2Send);
  rest.addRoute(GET, "/dish", dishSend);
  rest.addRoute(GET, "/lg2", lg2Send);
  rest.addRoute(GET, "/samsung2", samsung2Send);
  rest.addRoute(GET, "/sony2", sony2Send);
  rest.addRoute(GET, "/magiQuest", magiQuestSend);
  rest.addRoute(GET, "/rc5_2", rc5_2Send);
  rest.addRoute(GET, "/rc6_2", rc6_2Send);
  rest.addRoute(GET, "/nec2", nec2Send);
  rest.addRoute(GET, "/sharp2", sharp2Send);
  rest.addRoute(GET, "/whynter", whynterSend);
  rest.addRoute(GET, "/bosewave", bosewaveSend);
  rest.addRoute(GET, "/legoPowerFunction2", legoPowerFunction2Send);
  rest.addRoute(GET, "/panasonic2", panasonic2Send);
  rest.addRoute(GET, "/biPhase", biPhaseSend);
#endif /* MINIMUM */

#ifndef IR_SEND_PIN
  pinMode(IR_LED_PIN , OUTPUT);
  IrSender.begin((uint8_t) IR_LED_PIN);
#else /* IR_SEND_PIN */
  pinMode(IR_SEND_PIN , OUTPUT);
  IrSender.begin();
#endif /* IR_SEND_PIN */
}

void loop() {
  // Run the RestServer
  rest.run();
}

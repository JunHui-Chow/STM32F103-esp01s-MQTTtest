#ifndef	__ESP8266_H
#define	__ESP8266_H
#include "sys.h"
/*MQTT 连接参数*/
//#define clientId "a1Zw81YmcId.ESP8266|securemode=2\\,signmethod=hmacsha256\\,timestamp=1694524012615|"
//#define username "ESP8266&a1Zw81YmcId"
//#define passwd "a8bed5d35c6c689437c6ef2fa8d360044879460f6e6647fb4b8f985180ad42ca"
//#define mqttHostUrl "a1Zw81YmcId.iot-as-mqtt.cn-shanghai.aliyuncs.com"
//#define port "1883"
#define post "junhuitest"
#define post_name "junespcheck"

#define MQTT_set	"AT+MQTTUSERCFG=0,1,\"esp826601s\",\"esp826601s\",\"123\",0,0,\"\""
#define MQTT_Client "AT+MQTTCLIENTID=0,\"esp826601s\""
#define MQTT_Pass "AT+MQTTCONN=0,\"broker-cn.emqx.io\",1883,1"

#define WIFI_Name "espcheck"
#define WIFI_Pass "00000000"
extern unsigned char Property_Data[];		//数据保存数组
void ESP8266_Init(void);
unsigned char ESP8266_Re(unsigned char *ACK_AT);
void ESP8266_Send(char *property,int Data);
void ESP8266_Received(char *PRO);
u8 esp8266_send_cmd(char *cmd,char *ack,u16 waittime);
u8* esp8266_check_cmd(char *str);
#endif


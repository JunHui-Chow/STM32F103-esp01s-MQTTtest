#include "esp8266.h"
#include "usart2.h"
#include "string.h"
#include "delay.h"
unsigned char Property_Data[2];		//数据保存数组
/*****************************************************
*函数名称：void ESP8266_Init(void)
*函数功能：ESP8266参数初始化
*入口参数：void
*出口参数：void
*****************************************************/
void ESP8266_Init(void)
{
	//让Wifi模块重启的命令
	esp8266_send_cmd("AT+RST","ready",20);
    //配置WiFi工作模式为Station模式:ESP8266模块作为节点去连接wifi
	esp8266_send_cmd("AT+CWMODE=1","OK",50);


	delay_ms(1000);
	//让模块连接上自己的路由
	esp8266_send_cmd("AT+CWJAP=\"espcheck\",\"00000000\"","WIFI GOT IP",300);
	//AT+MQTTUSERCFG - 配置 MQTT 用户属性
	esp8266_send_cmd(MQTT_set,"OK",100);
	//配置 MQTT 客户端 ID
	esp8266_send_cmd(MQTT_Client,"OK",100);
	//连接指定 MQTT协议
	esp8266_send_cmd(MQTT_Pass,"OK",300);
    //订阅主题
    esp8266_send_cmd("AT+MQTTSUB=0,\"junhuitest\",1","OK",100);	
}

//向ESP8266发送命令
//cmd:发送的命令字符串;ack:期待的应答结果,如果为空,则表示不需要等待应答;waittime:等待时间(单位:10ms)
//返回值:0,发送成功(得到了期待的应答结果);1,发送失败
u8 esp8266_send_cmd(char *cmd,char *ack,u16 waittime)
{
	u8 res=0; 
	USART2_RX_STA=0;
	u2_printf("%s\r\n",cmd);
	if(ack&&waittime)		
	{
		while(--waittime)	
		{
			delay_ms(10);
			if(USART2_RX_STA&0X8000)
			{
				if(esp8266_check_cmd(ack))
				{
					printf("ack:%s\r\n",(u8*)ack);
					break;
				}
					USART2_RX_STA=0;
			}
		}
		if(waittime==0)res=1; 
	}
	return res;
} 
//ESP8266发送命令后,检测接收到的应答
//str:期待的应答结果
//返回值:0,没有得到期待的应答结果;其他,期待应答结果的位置(str的位置)

u8* esp8266_check_cmd(char *str)
{
	char *strx=0;
	if(USART2_RX_STA&0X8000)		//接收到一次数据了
	{ 
		USART2_RX_BUF[USART2_RX_STA&0X7FFF]=0;//添加结束符
		strx=strstr((const char*)USART2_RX_BUF,(const char*)str);
	} 
	return (u8*)strx;
}

/*****************************************************
*函数名称：ESP8266_Send(char *property,int Data)
*函数功能：向云端发送数据
*入口参数：char *property,int Data （属性）（数据）
*出口参数：void
*****************************************************/
void ESP8266_Send(char *property,int Data)
{
	USART2_RX_STA=0;
	u2_printf("AT+MQTTPUB=0,\"%s\",\"{{\\\"%s\\\":%d}}\",1,0\r\n",post,property,Data);
}

/*****************************************************
*函数名称：ESP8266_Received(void)
*函数功能：云端接收数据
*入口参数：char *property,int Data （属性）（数据）
*出口参数：void
*****************************************************/
void ESP8266_Received(char *PRO)
{
	unsigned char *ret = 0;
	char *property = 0;
	unsigned char i;
	if(PRO == NULL);
	else if(USART2_RX_STA&0x8000)			//串口2接收完一帧数据
	{
		ret = USART2_RX_BUF;
		if(ret!=0)
		{
			property = strstr((const char *)ret,(const char *)PRO);
			if(property!=NULL)
			{
				for(i=0;i<2;i++)
				{
					Property_Data[i] = *(property+12+i);
				}
				if(Property_Data[1] == '}')Property_Data[1]=0;
				USART2_RX_STA = 0;
			}
			else
			{
				USART2_RX_STA = 0;
			}
		}
		else
		{
			USART2_RX_STA = 0;	
		}
	}
}

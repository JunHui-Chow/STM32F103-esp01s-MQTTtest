#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "stdio.h"
#include "esp8266.h"
#include "timer.h"
//函数声明
void GPIO_Configuration(void);
int i=0,j=0;
int main(void)
{		
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	TIM3_Int_Init(10-1,7200-1);//10Khz的计数频率，计数到10为1ms
	uart_init(115200);	 //串口初始化为115200
	usart2_init(115200);	 //串口2初始化为115200
	ESP8266_Init();			//ESP8266初始化
	
	GPIO_Configuration();
	while(1)
	{
		if(Time_1ms%10==0)
		{
			ESP8266_Received(post_name);
			if(Property_Data[0] == '1')
			{
				PCout(13) = 0;
			}
			else
			{
				PCout(13) = 1;
			}
		}
		if(Time_1s>=3)
		{
			Time_1s	=	0;
			if(j==0)
			{
				j=1;
				ESP8266_Send("testflag",i++);
			}
			else if(j==1)
			{
				j=0;
				ESP8266_Send("brightness",i++);
			}
			if(i>99)i=0;
		}
	}
}
//=============================================================================
//文件名称：GPIO_Configuration
//功能概要：GPIO初始化
//参数说明：无
//函数返回：无
//=============================================================================
void GPIO_Configuration(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC , ENABLE); 						 
//=============================================================================
//LED -> PC13
//=============================================================================			 
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}




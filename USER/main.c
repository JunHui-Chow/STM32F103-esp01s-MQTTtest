#include "delay.h"
#include "sys.h"
#include "usart.h"
#include "usart2.h"
#include "stdio.h"
#include "esp8266.h"
#include "timer.h"
//��������
void GPIO_Configuration(void);
int i=0,j=0;
int main(void)
{		
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM3_Int_Init(10-1,7200-1);//10Khz�ļ���Ƶ�ʣ�������10Ϊ1ms
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
	usart2_init(115200);	 //����2��ʼ��Ϊ115200
	ESP8266_Init();			//ESP8266��ʼ��
	
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
//�ļ����ƣ�GPIO_Configuration
//���ܸ�Ҫ��GPIO��ʼ��
//����˵������
//�������أ���
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




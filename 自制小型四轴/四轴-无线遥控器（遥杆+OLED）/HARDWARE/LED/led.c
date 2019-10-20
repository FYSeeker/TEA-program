#include "led.h"



//LED IO��ʼ��
void LED_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz    		
	GPIO_Init(GPIOB, &GPIO_InitStructure);	  			
	GPIO_SetBits(GPIOB,GPIO_Pin_12); 						 
}
 

/**************************************************************************
�������ܣ�LED��˸
��ڲ�������˸Ƶ�� 
����  ֵ����
**************************************************************************/
void LED0_Flash(u16 time)
{
	static int temp;
	if(++temp==time)
		LED0=~LED0,temp=0;
}



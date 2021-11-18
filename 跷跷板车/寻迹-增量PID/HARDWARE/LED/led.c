#include "led.h"

/******************************************************************
*��������:	LED_Init
*��������:	��ʼ��LED
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ�ܶ˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		//50M
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	
	LED0 = 1;
	LED1 = 1;
}

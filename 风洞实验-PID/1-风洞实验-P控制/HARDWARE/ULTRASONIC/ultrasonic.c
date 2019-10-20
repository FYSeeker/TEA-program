#include "ultrasonic.h"
#include "lcd.h"
#include "timer.h"

u16 TIM2_Number=0;

void Ultrasonic_Init()	//��������ʼ����PA4����������
{
	GPIO_InitTypeDef GPIO_InitStructure;	//GPIO��ʼ���ṹ��
	
	RCC_APB2PeriphClockCmd(RCC_Ultrasonic_ECHO|RCC_Ultrasonic_TRIG, ENABLE);	//ʹ��IO��ʱ��
	
	GPIO_InitStructure.GPIO_Pin = PIN_Ultrasonic_TRIG;	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;		//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIO_Ultrasonic_TRIG, &GPIO_InitStructure);		//IO�ڳ�ʼ��
	
	GPIO_ResetBits(GPIO_Ultrasonic_TRIG, PIN_Ultrasonic_TRIG);	//�õ�
	
	GPIO_InitStructure.GPIO_Pin = PIN_Ultrasonic_ECHO;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			//��������
	GPIO_Init(GPIO_Ultrasonic_ECHO, &GPIO_InitStructure);	//IO�ڳ�ʼ��
}




float Ultrasonic_Ranging()		//���������
{	
	float value;
	
	TRIG = 0;
	TIM_Cmd(TIM2,ENABLE); 	//ʹ�ܶ�ʱ��2
	
	/*����ʱ��*/
	TRIG = 1;
	delay_us(15);
	TRIG = 0;
	
	while(nvic_flog!=2)		//�ȴ�������ɣ�ģ����յ�������
	{
		if(nvic_flog==3)	//��ʱ��������������10m
		{
			nvic_flog=0;	//����жϱ�־��׼����һ�β���
			return -1;
		}
	}
	nvic_flog=0;	//����жϱ�־��׼����һ�β���
	TIM_Cmd(TIM2,DISABLE);		//ʧ���ܶ�ʱ��2
	TIM_SetCounter(TIM2,0);

	LCD_ShowxNum(20,100,TIM2_Number,6,16,0X80);
	
	value = (float)TIM2_Number*0.017;	//������룬��λcm
	
	return value;
}





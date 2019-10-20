#include "ultrasonic.h"
#include "lcd.h"
#include "timer.h"

u8 nvic_flog = 0;

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
	u16 i;
	u16 TIM2_Number=0;
	float value;
	
	TRIG = 0;
	
	/*����ʱ��*/
	TRIG = 1;
	delay_us(12);
	TRIG = 0;
	
	while(!ECHO)
	{
		i++;
		if(i>65500)
		{
			i = 0;
			return -1;
		}
	}
	TIM_Cmd(TIM2,ENABLE); 	//ʹ�ܶ�ʱ��2
	
	while(ECHO)
	{
		if(nvic_flog==2)
		{
			nvic_flog=0;
			return -1;
		}		
	}
	TIM2_Number = TIM_GetCounter(TIM2);
	TIM_Cmd(TIM2,DISABLE);		//ʧ���ܶ�ʱ��2
	TIM_SetCounter(TIM2,0);

//	LCD_ShowxNum(20,150,TIM2_Number,6,16,0X80);
	
	value = (float)TIM2_Number*0.017;	//������룬��λcm
	
	return value;
}





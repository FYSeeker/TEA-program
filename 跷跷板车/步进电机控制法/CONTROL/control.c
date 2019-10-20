#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "inv_mpu.h"
#include "usart.h"
#include "motor.h"
#include "irtube.h"

u8 Run = 1;
short PWMValue=0;

_PID Pid = {12,0,0};		//170,0,1.5


/******************************************************************
*��������:	PID���ڣ�λ��ʽ
*��������:	Angle��ʵ�ʽǶ�
			AngularSpeed��ʵ�ʽ��ٶ�
			mode������ģʽ
*�� �� ֵ:	PWM��
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed,u8 mode)
{
	float value=0;
	static float Error=0;				//��ǰ���
	static float Integral=0;			//������
	
	Error = *Angle - 0.0;				//�������
	
	if(mode) Integral += Error;
	else Integral = 0;
	
	/*�����޷�*/
	if(Integral>Integral_MAX) Integral = Integral_MAX;
	else if(Integral<-Integral_MAX) Integral = -Integral_MAX;
	
	value = Pid.Kp * Error + Pid.Ki * Integral + Pid.Kd * (float)(*AngularSpeed);
	
	/*����޷�*/
	if(value>PWM_MAX)
	{
		value = PWM_MAX;
	}
	else if(value<-PWM_MAX)
	{
		value = -PWM_MAX;
	}
//	printf("%f\t%d\t%f\t",Error,*AngularSpeed,value);
	
	return (short)value;
}



/******************************************************************
*��������:	PWM������
*��������:	pwmvalue��PWM��ֵ
			mode������ģʽ
*�� �� ֵ:	��
*******************************************************************/
void PWM_Control(short pwmvalue)
{
	u8 mode=0;

	/*�����ж�*/
	if(pwmvalue<0)
	{
		mode = FRONT;
	}
	else
	{
		mode = REAR;
	}
	
	/*����Ѱ������*/
	if(mode==FRONT)					//ǰ��
	{
		if(IR1==0&&IR2==0)			//ǰ��ֱ��
		{			
			mode = FRONT;			//ǰ��
		}
		else if(IR1==0&&IR2==1)		//ǰ����ƫ
		{
			mode = RIGHT;			//��ת
		}
		else if(IR1==1&&IR2==0)		//ǰ����ƫ
		{
			mode = LEFT;			//��ת
		}
//		else if(IR1==1&&IR2==1)		//��⵽ֹͣ��
//		{
//			mode = STOP;			//ֹͣ
//			Run =  0;
//		}
	}
	else if(mode==REAR)				//����
	{
		if(IR3==0&&IR4==0)			//����ֱ��
		{			
			mode = REAR;			//����
		}
		else if(IR3==0&&IR4==1)		//������ƫ
		{
			mode = LEFT;			//��ת
		}
		else if(IR3==1&&IR4==0)		//������ƫ
		{
			mode = RIGHT;			//��ת
		}
//		else if(IR3==1&&IR4==1)		//��⵽ֹͣ��
//		{
//			mode = STOP;			//ֹͣ
//			Run =  0;
//		}
	}
	
	Motor_Control(mode);
	
//	printf("%d\t%d\r\n",pwmvalue,mode);
}


/******************************************************************
*��������:	��ʼ����ʱ��3,�����ж�
*��������:	arr���Զ���װֵ
			psc����Ƶϵ��
*�� �� ֵ:	��
*******************************************************************/
void TIM3_IntInit(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	/*��ʼ����ʱ������*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);					//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	/*��ʼ����ʱ���ж�*/
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);						//��ʱ�������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM3, ENABLE);
}


/******************************************************************
*��������:	��ʱ��3�ĸ����ж�,1ms
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM3_IRQHandler(void)
{	
	static u16 Tim3Int=0;
	
	if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ	
		
		Tim3Int++;
		if(Tim3Int>999) Tim3Int = 0;
	}
}


/******************************************************************
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/*��ʼ����ʱ������*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	/*��ʼ����ʱ���ж�*/
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);						//��ʱ�������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;		//��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				//�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM1, ENABLE);
}


/******************************************************************
*��������:	TIM1_UP_IRQHandler
*��������:	��ʱ��1�ĸ����ж�,5ms
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 Tim1Int=0;
	static float Pitch=0;
	static float Roll=0;
	static float Yaw=0;
	static short X_AngularSpeed=0;
	static short Y_AngularSpeed=0;
	static short Z_AngularSpeed=0;
	static u16 PWMTim=0;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ	
		
		if(Run)
		{
			if(mpu_dmp_get_data(&Pitch,&Roll,&Yaw,&X_AngularSpeed,&Y_AngularSpeed,&Z_AngularSpeed)==0)
			{	
				PWMValue = PID_Adjust(&Pitch,&X_AngularSpeed,1);
				
				if(PWMValue<0)
				{
					PWMTim = -PWMValue;
				}
				else
				{					
					PWMTim = PWMValue;
				}
				
				if(Tim1Int<PWMTim)
				{
					PWM_Control(PWMValue);				
				}
				else
				{					
					Motor_Control(0);
				}
			}
			Tim1Int++;
			if(Tim1Int>99) Tim1Int = 0;
		}
		else
		{
			Tim1Int = 0;
			PID_Adjust(0,0,0);		//��ջ���
			Motor_Control(0);
		}
		
	}
}


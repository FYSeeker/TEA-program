#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "inv_mpu.h"
#include "usart.h"
#include "motor.h"
#include "irtube.h"

u8 Run = 0;

_PID Pid = {0.188,1,0};


/******************************************************************
*��������:	PID���ڣ�����ʽ
*��������:	Angle��ʵ�ʽǶ�
			AngularSpeed��ʵ�ʽ��ٶ�
*�� �� ֵ:	PWM��
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed)
{
	float value=0;
	static float Error=0;				//��ǰ���
	static short Last_AngularSpeed=0;	//�ϴν��ٶ�
	
	Error = *Angle - 0.0;					//�������
	
	value = Pid.Kp * (float)(*AngularSpeed) + Pid.Ki * Error + Pid.Kd * (float)((*AngularSpeed) - Last_AngularSpeed);
	
	Last_AngularSpeed = *AngularSpeed;	//�ϴν��ٶ�
	
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

	if(pwmvalue<0)
	{
		pwmvalue = -pwmvalue;
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
	
	Motor_Control(pwmvalue,pwmvalue,mode);
	
//	printf("%d\t%d\r\n",pwmvalue,mode);
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
	static float Pitch=0;
	static float Roll=0;
	static float Yaw=0;
	static short X_AngularSpeed=0;
	static short Y_AngularSpeed=0;
	static short Z_AngularSpeed=0;
	static short PWMValue=0;
	static short PWMMAX = 6000;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ	
		
		if(Run)
		{
			if(mpu_dmp_get_data(&Pitch,&Roll,&Yaw,&X_AngularSpeed,&Y_AngularSpeed,&Z_AngularSpeed)==0)
			{	
				PWMValue += PID_Adjust(&Pitch,&X_AngularSpeed);
				
				/*����޷�*/
				if(PWMValue>PWMMAX)
				{
					PWMValue = PWMMAX;
				}
				else if(PWMValue<-PWMMAX)
				{		
					PWMValue = -PWMMAX;
				}	
				
				PWM_Control(PWMValue);
			}
		}
		else
		{
			PWMValue = 0;
			Motor_Control(0,0,0);
		}
	}
}


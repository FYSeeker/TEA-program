#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "inv_mpu.h"
#include "usart.h"
#include "motor.h"


/*PID����*/
_PID Pid = {0,0,0};		//170,0,1.5


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
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	arr���Զ���װֵ
			psc����Ƶϵ��
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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM1, ENABLE);
}


u8 USART3_flag = 0;

/******************************************************************
*��������:	TIM1_UP_IRQHandler
*��������:	��ʱ��1�ĸ����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 PwmValue = 4500;
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ	
		
		if((USART3_flag&0x0f)!=0)		//�յ�ָ��
		{
			switch(USART3_flag&0x0f)
			{
				case 0x01:Motor_Control(PwmValue,PwmValue,FRONT);	//ǰ��
					break;
				case 0x02:Motor_Control(PwmValue,PwmValue,REAR);	//����
					break;
				case 0x03:Motor_Control(PwmValue,PwmValue,LEFT);	//��ת
					break;
				case 0x04:Motor_Control(PwmValue,PwmValue,RIGHT);	//��ת
					break;
				default:Motor_Control(0,0,STOP);
			}
			
			USART3_flag &= 0xf0;	//���
		}
		else
		{
			Motor_Control(0,0,STOP);
		}
	}
}


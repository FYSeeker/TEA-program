#include "motor.h"


/******************************************************************
*��������:	MotorPwmInit
*��������:	��ʼ�����ڵ����PWM��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void MotorPWMInit(u16 arr,u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);		//ʹ��TIM2ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//ʹ��GPIO����ʱ��ʹ��
	
	/*��ʼ����ʱ��IO��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;	//CH1,CH2,CH3,CH4
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;										//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/*��ʼ����ʱ������*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 
	TIM_TimeBaseStructure.TIM_Prescaler = psc;					//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	/*��ʼ����ʱ��PWM��*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);						//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);						//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);						//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4Init(TIM2, &TIM_OCInitStructure);						//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_CtrlPWMOutputs(TIM2, ENABLE);					//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH1Ԥװ��ʹ��	 
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH2Ԥװ��ʹ��	
	TIM_OC3PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH3Ԥװ��ʹ��	 
	TIM_OC4PreloadConfig(TIM2, TIM_OCPreload_Enable);	//CH4Ԥװ��ʹ��	 

	TIM_ARRPreloadConfig(TIM2, ENABLE);					//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���

	TIM_Cmd(TIM2, ENABLE);								//ʹ�ܶ�ʱ��
}

#include "motor.h"


/******************************************************************
*��������: TIM8_PWM_init
*�� �� ֵ: ��
*��������: arr:�Զ���װֵ
		   psc:�Զ�Ԥ��Ƶ��
*��������: ��ʼ����ʱ��8,����PWM��(����4·)
*******************************************************************/
void TIM8_PWM_init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8 | RCC_APB2Periph_GPIOC, ENABLE);	//ʱ��ʹ��
	
	//���ø�����Ϊ�����������,���TIM8-CH1,CH2,CH3,CH4��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9;	//CH1,CH2,CH3,CH4	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;			//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	TIM_TimeBaseStructure.TIM_Period = arr;			//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler = psc;		//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);	//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;								//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;		//�������:TIM����Ƚϼ��Ը�
	
	TIM_CtrlPWMOutputs(TIM8,ENABLE);					//MOE �����ʹ��	
	
	TIM_OC1Init(TIM8, &TIM_OCInitStructure);			//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC1PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIMx��CCR1�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC2Init(TIM8, &TIM_OCInitStructure);			//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);			//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	
	TIM_OC4Init(TIM8, &TIM_OCInitStructure);			//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC4PreloadConfig(TIM8, TIM_OCPreload_Enable);	//ʹ��TIMx��CCR4�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM8, ENABLE);		//ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM_Cmd(TIM8, ENABLE);					//ʹ��TIMx����
}




/**************************************************************************************
�������ܣ�	С������
��ڲ�����	LeftPwm����ߵ��PWMֵ
			RightPwm���ұߵ��PWMֵ
			Mode������ģʽ
���ز�����	��
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode)
{	
	switch(Mode)
	{
		/*ǰ��*/
		case FRONT:		
				PWM1 = 0;
				PWM2 = LeftPwm;
				PWM3 = RightPwm;
				PWM4 = 0;
			break;
		
		/*����*/
		case REAR:	
				PWM1 = LeftPwm;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = RightPwm;
			break;
		
		/*��ת*/
		case LEFT:
				PWM1 = LeftPwm;
				PWM2 = 0;
				PWM3 = RightPwm;
				PWM4 = 0;
			break;
		
		/*��ת*/
		case RIGHT:
				PWM1 = 0;
				PWM2 = LeftPwm;
				PWM3 = 0;
				PWM4 = RightPwm;
			break;
		
		/*ֹͣ*/
		case STOP:
				PWM1 = 0;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = 0;
			break;
		
		default:PWM1 = 0;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = 0;
	}
}



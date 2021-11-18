#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

/*��ʱ���ȽϼĴ���*/
#define PWM1 TIM2->CCR1		//PA0
#define PWM2 TIM2->CCR2		//PA1
#define PWM3 TIM2->CCR3		//PA2
#define PWM4 TIM2->CCR4		//PA3

#define FRONT 1		//ǰ
#define REAR  2		//��
#define LEFT  3		//��
#define RIGHT 4		//��
#define STOP  5		//ֹͣ

/***************************************************************************
�������ܣ�	TIM��PWM��ʼ��(����4·)
��ڲ�����	arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
���ز�����	��
****************************************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc);

/**************************************************************************************
�������ܣ�	С������
��ڲ�����	LeftPwm����ߵ��PWMֵ
			RightPwm���ұߵ��PWMֵ
			Mode������ģʽ
���ز�����	��
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode);

#endif


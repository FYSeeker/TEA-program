#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

#define PWM1 TIM2->CCR1		//PA0
#define PWM2 TIM2->CCR2		//PA1
#define PWM3 TIM2->CCR3		//PA2
#define PWM4 TIM2->CCR4		//PA3

#define FRONT 1		//ǰ
#define REAR  2		//��
#define LEFT  3		//��
#define RIGHT 4		//��
#define STOP  5		//ֹͣ

/******************************************************************
*��������: TIM2_PWM_init
*�� �� ֵ: ��
*��������: arr:�Զ���װֵ
		   psc:�Զ�Ԥ��Ƶ��
*��������: ��ʼ����ʱ��2,����PWM��(����4·)
*******************************************************************/
void TIM2_PWM_init(u16 arr, u16 psc);

/**************************************************************************************
�������ܣ�	С������
��ڲ�����	LeftPwm����ߵ��PWMֵ
			RightPwm���ұߵ��PWMֵ
			Mode������ģʽ
���ز�����	��
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode);

#endif


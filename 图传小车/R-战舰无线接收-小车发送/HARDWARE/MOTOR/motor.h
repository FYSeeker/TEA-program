#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

/*PWM��*/
#define PWM1 TIM8->CCR1		//PC6
#define PWM2 TIM8->CCR2		//PC7
#define PWM3 TIM8->CCR3		//PC8
#define PWM4 TIM8->CCR4		//PC9

/*С������ģʽ*/
#define FRONT 1		//ǰ
#define REAR  2		//��
#define LEFT  3		//��
#define RIGHT 4		//��
#define STOP  5		//ֹͣ

/******************************************************************
*��������: TIM8_PWM_init
*�� �� ֵ: ��
*��������: arr:�Զ���װֵ
		   psc:�Զ�Ԥ��Ƶ��
*��������: ��ʼ����ʱ��8,����PWM��(����4·)
*******************************************************************/
void TIM8_PWM_init(u16 arr, u16 psc);

/**************************************************************************************
�������ܣ�	С������
��ڲ�����	LeftPwm����ߵ��PWMֵ
			RightPwm���ұߵ��PWMֵ
			Mode������ģʽ
���ز�����	��
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode);

#endif


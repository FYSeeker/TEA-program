#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

#define PWM1	TIM2->CCR1	//PA0
#define PWM2	TIM2->CCR2	//PA1
#define PWM3	TIM2->CCR3	//PA2
#define PWM4	TIM2->CCR4	//PA3


/******************************************************************
*��������:	MotorPwmInit
*��������:	��ʼ�����ڵ����PWM��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void MotorPWMInit(u16 arr,u16 psc);

#endif

#ifndef MOTOR_H_
#define MOTOR_H_

#include "sys.h"

#define PWMA	TIM3->CCR4	//PB1
#define PWMB	TIM3->CCR3	//PB0
#define AIN1	PBout(14)
#define AIN2	PBout(15)
#define BIN1	PBout(13)
#define BIN2	PBout(12)

/******************************************************************
*��������:	MotorInit
*��������:	��ʼ���������������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void MotorInit(void);

/******************************************************************
*��������:	MotorPwmInit
*��������:	��ʼ�����ڵ������·PWM��,Ƶ��Ϊ10KHZ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void MotorPwmInit(void);

#endif

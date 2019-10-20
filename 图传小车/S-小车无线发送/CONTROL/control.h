#ifndef CONTROL_H_
#define CONTROL_H_

#include "sys.h"

#define Integral_MAX 10000
#define PWM_MAX 5000

extern u8 USART3_flag;
extern u8 Wait_falg;

typedef struct
{
	float Kp;				//����ϵ��
	float Ki;				//����ϵ��
	float Kd;				//΢��ϵ��
}_PID;

extern _PID Pid;


/******************************************************************
*��������:	PID���ڣ�����ʽ
*��������:	Angle��ʵ�ʽǶ�
			AngularSpeed��ʵ�ʽ��ٶ�
			mode������ģʽ
*�� �� ֵ:	PWM��
*******************************************************************/
short PID_Adjust(float* Angle,short* AngularSpeed,u8 mode);

/******************************************************************
*��������:	PWM������
*��������:	pwmvalue��PWM��ֵ
			mode������ģʽ
*�� �� ֵ:	��
*******************************************************************/
void PWM_Control(short pwmvalue);

/******************************************************************
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);

#endif

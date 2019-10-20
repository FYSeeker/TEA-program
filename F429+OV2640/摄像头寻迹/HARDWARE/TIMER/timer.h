#ifndef _TIMER_H
#define _TIMER_H

#include "sys.h"

#define Integral_MAX 10000
#define PWM_MAX 5000

typedef struct
{
	float Kp;				//����ϵ��
	float Ki;				//����ϵ��
	float Kd;				//΢��ϵ��
}_PID;

extern u8 Run;
extern u8 Center_flag;
extern u16 MidlinePos;

/***************************************************************************
�������ܣ�	��ʱ���жϳ�ʼ��
��ڲ�����	arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
���ز�����	��
����˵��;	��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
			Ft=��ʱ������Ƶ��,��λ:Mhz
			��ʱ��1����APB2�ϣ�ʱ��ΪAPB2*2=180MHz
****************************************************************************/
void TIM1_Init(u16 arr,u16 psc);

#endif


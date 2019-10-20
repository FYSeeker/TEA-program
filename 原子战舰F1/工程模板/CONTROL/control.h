#ifndef _CONTROL_H_
#define _CONTROL_H_

#include "sys.h"


#define PWM_MAX_VALUE	6500	
#define MAX_INTEGRAL	10000	

typedef struct Pid
{
	float Kp;
	float Ki;
	float Kd;
} PID;


/******************************************************************
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);


#endif

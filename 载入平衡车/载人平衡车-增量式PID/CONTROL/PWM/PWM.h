#ifndef _PWM_H_
#define _PWM_H_

#include "sys.h"

void MOTO_DIR_Init(void);					//����IO�ڳ�ʼ��
void TIM2_PWM_Init(u16 arr,u16 psc);		//pwm��ʼ��
void Balance_PWM_Init(u16 arr,u16 psc);		//pwm��io�ڷ����ʼ��

#endif

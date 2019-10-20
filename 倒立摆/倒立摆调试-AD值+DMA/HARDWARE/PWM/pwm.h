#ifndef PWM_H_
#define PWM_H_

#include "sys.h"

#define PWM1 TIM3->CCR1	//PA6
#define PWM2 TIM3->CCR2	//PA7

void TIM1_PWM_init(u16 arr, u16 psc);
void TIM2_PWM_init(u16 arr, u16 psc);
void TIM3_PWM_init(u16 arr, u16 psc);
void TIM4_PWM_init(u16 arr, u16 psc);
void TIM8_PWM_init(u16 arr, u16 psc);

/******************************************************************
*��������:	Tim4EncoderInit
*��������:	��ʼ����ʱ��4,���ڱ�����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Tim4EncoderInit(void);

#endif

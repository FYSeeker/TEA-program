#ifndef _BLANCE_H_
#define _BLANCE_H_

#include "sys.h"

#define PWM_L   TIM2->CCR1		//PA0
#define DIR_L   PBout(11)

#define PWM_R   TIM2->CCR2		//PA1
#define DIR_R   PBout(10)

typedef struct 
{
	float P;
	float I;
	float D;
	
}PID;

extern u8 ARMED;		//����������־λ
extern PID BlancePid;	//ƽ�⻷pid
extern u8 Accelerator;	//�ٶȵ�λ��1��2��3


#endif

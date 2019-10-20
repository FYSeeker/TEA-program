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


extern u8 Run;
extern short AimAngle;
extern u16 IntTime;
extern float Angle;

extern PID upPid;
extern PID SpeedPid;


/**********************************************************************
�������ܣ�����ADC��ֵ����Ƕ�
��ڲ�����ADC��ֵ
���ز������Ƕ�
***********************************************************************/
float Count_Angle(u16 value);

/******************************************************************
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);

short Get_EncoderValue(void);

short Upright_PID(short AimAngle,float ActualAngle,u8 Mode);

short Speed_PID(short EncoderValue,u8 Mode);

void Procedure_1(void);
void Procedure_2(void);
void Procedure_3(void);

#endif

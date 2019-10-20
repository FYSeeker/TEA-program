#include "pid.h"

_PID pid;

void PID_Init()
{
	pid.actual_value=0;		//ʵ��ֵ
	pid.expect_value=20;	//����ֵ
	pid.error_value=0;		//���ֵ
	pid.Kp=0.5;
}

float PID_Adjust(float actual)	//PID����
{
	float value;
	
	PID_Init();
	
	pid.actual_value = actual;
	pid.error_value = pid.actual_value - pid.expect_value;	//�������
	value = pid.Kp*pid.error_value;
	
	return value;
}

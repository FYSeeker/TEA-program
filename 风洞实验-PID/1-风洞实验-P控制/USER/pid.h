#ifndef _pid_h_
#define _pid_h_


typedef struct
{
	float actual_value;	//ʵ��ֵ
	float expect_value;	//����ֵ
	float error_value;	//���ֵ
	float Kp;			//����ϵ��
}_PID;

void PID_Init(void);

float PID_Adjust(float actual); 

#endif

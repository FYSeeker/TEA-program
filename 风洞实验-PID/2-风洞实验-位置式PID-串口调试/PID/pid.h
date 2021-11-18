#ifndef _pid_h_
#define _pid_h_

typedef struct
{
	double expect;		//����ֵ
	double Kp;			//����ϵ��
	double Ki;			//����ϵ��
	double Kd;			//΢��ϵ��
}_PID;


extern _PID pid;

void PID_Init(void);

double PID_Adjust(double actual); 

#endif

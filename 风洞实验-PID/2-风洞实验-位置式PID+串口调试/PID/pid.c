#include "pid.h"

_PID pid;

void PID_Init()
{
	pid.expect= 20;		//����ֵ
	
	pid.Kp = 10;	//10
	pid.Ki = 0.1;	//0.1
	pid.Kd = 0;		//
}

double PID_Adjust(double actual)	//PID����,λ��ʽ
{
	double value;
	static double error;			    //���ֵ��������
	static double error_change = 0;		//���仯�ʣ�΢�֣�
	static double error_last = 0;		//�ϴ����
	static double error_add = 0;		//�ۼ����ֵ�����֣�
	
	error = actual - pid.expect;	//���㱾�����
	
	error_add += error;		//�������
	if(error_add>20)  error_add=20;		//�����޷�
	if(error_add<-20)  error_add=-20;
	
	error_change  = error - error_last;		//����΢��
	
	value = pid.Kp*error + pid.Ki*error_add + pid.Kd*error_change;
	
	error_last = error;		//��¼�ϴ����
	
	return value;
}

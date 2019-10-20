#include "pid.h"
#include "lcd.h"
#include "usart.h"

_PID pid;

void PID_Init()
{
	pid.expect = 20;	//����ֵ
	
	pid.Kp = 0;		//12.3
	pid.Ki = 0;		//0.045
	pid.Kd = 0;		//0
}


double PID_Adjust(double actual)	//PID����,����ʽ
{
	double value;
	static double error;			    //��ǰ���
	static double error_last1 = 0;		//�ϴ����
	static double error_last2 = 0;		//���ϴ����
	static double error_change = 0;		//���仯�ʣ�΢�֣�
	
	error = actual - pid.expect;	//���㱾�����
	
	error_change = error - 2*error_last1 - error_last2;	//����΢��
	
	value = pid.Kp*(error - error_last1) + pid.Ki*error + pid.Kd*error_change;
	
	error_last2 = error_last1;		//��¼���ϴ����
	error_last1 = error;			//��¼�ϴ����
	
	return value;
}


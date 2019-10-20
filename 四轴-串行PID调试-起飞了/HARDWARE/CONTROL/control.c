#include "control.h"
#include "filter.h"
#include "pwm.h"
#include "timer.h"
#include "led.h"
#include "usart.h"
#include "delay.h"
#include "mpu6050.h"
#include "inv_mpu.h"

u8  ARMED=0;		// ���������־  0 - ����  1 - ����
u8  FINISH;			// ��ȡ���ݱ�־  0 - ���  1 - ʧ��
u16 THROTTLE=1672;	// ���ſ�����  �޸ĺ��ֵ��Ҫʵ��
u8  TIME_flag;		// ʱ���־

     
/*---- �ڻ� P, I, D ----*/
PID  INNER_X = {0.03, 0.01, 0.16},
     INNER_Y = {0.03, 0.01, 0.16},
     INNER_Z = {0.01, 0.01, 0.10};
/*---- �⻷ P, I, D ----*/
PID  OUTER_X = {8, 0, 2},
     OUTER_Y = {8, 0, 2},
     OUTER_Z = {0, 0, 0};
     
	
SITE  Angle					= {0, 0, 0};	// ŷ����
SITE  Expected_Angle		= {0, 0, 0};	// ����ŷ����
SITE  Angle_Integral		= {0, 0, 0};	// ŷ���ǻ���
SITE  Palstance				= {0, 0, 0};	// ���ٶ�
SITE  Expected_Palstance	= {0, 0, 0};	// �������ٶ�
SITE  Palstance_Integral	= {0, 0, 0};	// ���ٶȻ���



void TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)  //5ms��ʱ�ж�
	{   
		TIM1->SR&=~(1<<0);							// �����ʱ��1�жϱ�־λ
	
		TIME_flag++;  								// ʱ���־
		if(ARMED)  // ���ڽ���״̬
		{
			LED1_Flash(100);
			FINISH = MPU6050_data_filter(&Angle, &Palstance);	// ��ȡ���� ���˲�����  ʱ�䣺2.76ms
			
			if(FINISH==0)  // ȷ��������ȷ  �����ڽ���״̬  ʱ�䣺52us
			{
				OUTER_CONTROL(Angle, Expected_Angle);  			// �⻷����  ʱ�䣺22us
				INNER_CONTROL(Palstance, Expected_Palstance);	// �ڻ�����  ʱ�䣺26us
			}
		}
		else  // ��������״̬
		{
			LED0_Flash(100);
			Run_Off();  // �رյ��
		}
	}       	
} 


/*********************************************************************
* �������ܣ��ڻ�PID���� -- ���Ƶ��
* ��ڲ��������ٶ�ֵ  �������ٶ�ֵ
* ��    �ߣ����������Ӽ���Э��
**********************************************************************/
void INNER_CONTROL(SITE palstance, SITE expected_palstance)  // �ڻ�����
{
	u16 moto1=0,moto2=0,moto3=0,moto4=0;
	static SITE Elast_palstance = {0, 0, 0};  // �����ϴν��ٶ�
	SITE Epalstance;	// ��ֵ
	SITE Edpalstance;	// ���΢��
	MOTOR X, Y, Z;  	// PID�������
	
	/*-- ��ǰ���ٶ���� --*/
	Epalstance.x = palstance.x - expected_palstance.x;
	Epalstance.y = palstance.y - expected_palstance.y;
	Epalstance.z = palstance.z - expected_palstance.z;
	
	/*-- ��ǰ���ٶ������� --*/
	Palstance_Integral.x += Epalstance.x;  
	Palstance_Integral.y += Epalstance.y;
	Palstance_Integral.z += Epalstance.z;
	integral_limit(&Palstance_Integral, 0);  // ��������
	
	/*-- ��ǰ���ٶ����΢�� --*/
	Edpalstance.x = Epalstance.x - Elast_palstance.x;  
	Edpalstance.y = Epalstance.y - Elast_palstance.y;
	Edpalstance.z = Epalstance.z - Elast_palstance.z;
	Elast_palstance.x = Epalstance.x;  // �����ϴν��ٶ����
	Elast_palstance.y = Epalstance.y;
	Elast_palstance.z = Epalstance.z;
	
	/*-- X�� --*/
	X.pout = INNER_X.P * Epalstance.x;
	X.iout = INNER_X.I * Palstance_Integral.x;
	X.dout = INNER_X.D * Edpalstance.x;
	/*-- Y�� --*/
	Y.pout = INNER_Y.P * Epalstance.y;
	Y.iout = INNER_Y.I * Palstance_Integral.y;
	Y.dout = INNER_Y.D * Edpalstance.y;
	/*-- Z�� --*/
	Z.pout = INNER_Z.P * Epalstance.z;
	Z.iout = INNER_Z.I * Palstance_Integral.z;
	Z.dout = INNER_Z.D * Edpalstance.z;

	/*-- ����������� --*/
	X.OUT = X.pout + X.iout + X.dout;
	Y.OUT = Y.pout + Y.iout + Y.dout;
	Z.OUT = Z.pout + Z.iout + Z.dout;

	if(THROTTLE>1670)  //����С��1670�����
	{
		moto1 = THROTTLE + X.OUT - Y.OUT + Z.OUT;
		moto2 = THROTTLE + X.OUT + Y.OUT - Z.OUT;
		moto3 = THROTTLE - X.OUT + Y.OUT + Z.OUT;
		moto4 = THROTTLE - X.OUT - Y.OUT - Z.OUT;
	}
	else  // �رյ��
	{
		moto1 = 1200; 
		moto2 = 1200; 
		moto3 = 1200; 
		moto4 = 1200; 
		Integral_Reset();  // ��������
	}
	
	if(Angle.x >= 45 || Angle.x <= -45 || Angle.y >= 45 || Angle.y <= -45)ARMED=0;  // ����������ȫ
	
	Moto_PwmRflash(moto1, moto2, moto3, moto4);  // ����ռ�ձ�
}


/*********************************************************************
* �������ܣ��⻷PID���� -- �ó��������ٶ�
* ��ڲ������Ƕ�ֵ  �����Ƕ�ֵ
* ��    �ߣ����������Ӽ���Э��
**********************************************************************/
void OUTER_CONTROL(SITE angle, SITE expected_angle)  // �⻷����
{
	static SITE Eangle_angle = {0, 0, 0};  // �����ϴνǶ�
	SITE Eangle;	// ��ֵ
	SITE Edangle;	// ���΢��
	
	/*-- ��ǰ�Ƕ���� --*/
	Eangle.x = angle.x - expected_angle.x;
	Eangle.y = angle.y - expected_angle.y;
	Eangle.z = angle.z - expected_angle.z;

	/*-- ��ǰ�Ƕ������� --*/
	Angle_Integral.x += Eangle.x;
	Angle_Integral.y += Eangle.y;
	Angle_Integral.z += Eangle.z;
	integral_limit(&Angle_Integral, 1);  // ��������

	/*-- ��ǰ�Ƕ����΢�� --*/
	Edangle.x = Eangle.x - Eangle_angle.x;
	Edangle.y = Eangle.y - Eangle_angle.y;
	Edangle.z = Eangle.z - Eangle_angle.z;
	Eangle_angle.x = Eangle.x; // �����ϴνǶ����
	Eangle_angle.y = Eangle.y;
	Eangle_angle.z = Eangle.z;

	/*-- �ó��������ٶ� --*/
	Expected_Palstance.x = -(Eangle.x*OUTER_X.P + Angle_Integral.x*OUTER_X.I + Edangle.x*OUTER_X.D);
	Expected_Palstance.y = -(Eangle.y*OUTER_Y.P + Angle_Integral.y*OUTER_Y.I + Edangle.y*OUTER_Y.D);
	Expected_Palstance.z = -(Eangle.z*OUTER_Z.P + Angle_Integral.z*OUTER_Z.I + Edangle.z*OUTER_Z.D);
}

void Moto_PwmRflash(u16 m1, u16 m2, u16 m3, u16 m4)	// ����ռ�ձ�
{
	if(m1>3500)m1=3500;  // ����
	if(m2>3500)m2=3500;
	if(m3>3500)m3=3500;
	if(m4>3500)m4=3500;
	PWM1 = m1;
	PWM2 = m2;
	PWM3 = m3;
	PWM4 = m4;
}

void Run_Off(void)  // �رյ��
{
	PWM1 = 1200, PWM2 = 1200, PWM3 = 1200, PWM4 = 1200;
	Integral_Reset();  // ��������
}

void Integral_Reset(void)  // ��������
{
	Angle_Integral.x = 0, Angle_Integral.y = 0, Angle_Integral.z = 0;  // ��������
	Palstance_Integral.x = 0, Palstance_Integral.y = 0, Palstance_Integral.z = 0;
}

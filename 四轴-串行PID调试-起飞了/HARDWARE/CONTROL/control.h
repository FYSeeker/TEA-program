#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern u8  ARMED;		// ����Ƿ�������־λ
extern u16 THROTTLE;	// ���ſ�����
extern u8  TIME_flag;	// ʱ���־


//PID ����
typedef struct
{
	float P;
	float I;
	float D;
}PID;

//XYZ ��������
typedef struct
{
	float x;
	float y;
	float z;
}SITE;

//PID �������
typedef struct
{
	float pout;
	float iout;
	float dout;
	s16   OUT;
}MOTOR;

/*---- �⻷ P, I, D ----*/
extern PID OUTER_X, OUTER_Y, OUTER_Z;
/*---- �ڻ� P, I, D ----*/
extern PID INNER_X, INNER_Y, INNER_Z;

extern SITE Angle;				// ŷ����
extern SITE Expected_Angle;		// ����ŷ����
extern SITE Angle_Integral;		// ŷ���ǻ��� 
extern SITE Palstance;			// ���ٶ�
extern SITE Expected_Palstance;	// �������ٶ�
extern SITE Palstance_Integral;	// ���ٶȻ��� 

void Moto_PwmRflash(u16 m1, u16 m2, u16 m3, u16 m4);			// ����ռ�ձ�
void Run_Off(void);												// �رյ��
void Integral_Reset(void);										// ��������
void OUTER_CONTROL(SITE angle, SITE expected_angle);			// �⻷����
void INNER_CONTROL(SITE palstance, SITE expected_palstance);  	// �ڻ�����

#endif

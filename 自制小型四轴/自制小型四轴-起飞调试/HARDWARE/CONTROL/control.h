#ifndef CONTROL_H_
#define CONTROL_H_

#include "sys.h"

#define THRESHOLD		3000	//���ŷ�ֵ
#define MAX_INTEGRAL	1000	//�����޷����ֵ
#define X_PWM_MAX		7200	//X��PWM���޷�
#define Y_PWM_MAX		7200	//Y��PWM���޷�
#define Z_PWM_MAX		4000	//Z��PWM���޷�

extern u8 LOCK_FLAG;			//������־

extern u16 Accelerator;	//����ֵ

/*PID����*/
typedef struct
{
	float P;
	float I;
	float D;
}PID;


/*����������*/
typedef struct
{
	float pitch;
	float roll;
	float yaw;
	short X_AngularSpeed;
	short Y_AngularSpeed;
	short Z_AngularSpeed;
}MPUDATA;


/*X,Y,Z����*/
typedef struct
{
	float X;
	float Y;
	float Z;
}XYZ_DATA;

extern PID Pid_X;
extern PID Pid_Y;
extern PID Pid_Z;

/******************************************************************
*��������:	CheckState
*��������:	��������״̬,��������Ǿ���ֵ����40��,ֹͣ���
*��������:	pitch:	������;roll: �����
*�� �� ֵ:	0:		״̬����
*			����:	״̬�쳣
*******************************************************************/
u8 CheckState(float pitch, float roll);


/******************************************************************
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void PID_Control(MPUDATA MpuData, MPUDATA MpuAngularSpeed);


/******************************************************************
*��������:	Descend
*��������:	�����½���������ˤ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Descend(void);


/******************************************************************
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc);

#endif

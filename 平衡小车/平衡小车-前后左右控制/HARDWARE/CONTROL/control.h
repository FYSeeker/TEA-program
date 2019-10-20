#ifndef CONTROL_H_
#define CONTROL_H_

#include "sys.h"

#define PWM_MAX_VALUE	6000	//PWM���޷�,���Ϊ7200
#define MAX_INTEGRAL	3500	//�����޷����ֵ
#define	MAX_PI			5500	//PI�ٶȻ����PWM�����ֵ

/************************SpeedPi()����ר��************************/
#define NORMAL_MODE		0		//��������
#define EMPTY_INTEGRAL	1		//��ջ���
/*****************************************************************/

/**********************SaveAndRead()����ר��**********************/
#define SAVE_DATA		0		//��������
#define READ_DATA		1		//��ȡ����
#define EMPTY_DATA		2		//�������
/*****************************************************************/

typedef struct Pid
{
	float kp;
	float ki;
	float kd;
} PID;

extern PID upPD;		//ֱ��PD����
extern PID velocityPI;	//�ٶ�PI����
extern s8 LeftSteer;
extern s8 RightSteer;
extern s8 Steer;
extern s16 TurnValue;

/******************************************************************
*��������:	KeyStartAndStop
*��������:	�������ڿ����͹ر�С��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void KeyStartAndStop(void);

/***********************************************************************
*��������:	SaveAndRead
*��������:	���ڱ���Ͷ�ȡ��Ҫ������(������,Y����ٶ�,��߱���������,
*			�ұ߱���������),������ȫ�ֱ���������,���ͳ�����϶�
*��������:	pitch:				ָ�����ǵ�ָ��
*			yAngularSpeed:		ָ��Y����ٶȵ�ָ��
*			encoderLeftNum:		ָ����߱�����������ָ��
*			encoderRightNum:	ָ���ұ߱�����������ָ��
*			mode:				ģʽ(SAVE_DATAΪ��������,
*								READ_DATAΪ��ȡ����,
*								EMPTY_DATAΪ�������)
*�� �� ֵ:	��
************************************************************************/
void SaveAndRead(float *pitch, short *yAngularSpeed, short *encoderLeftNum, short *encoderRightNum, u8 mode);

/******************************************************************
*��������:	Tim3TimeIntInit
*��������:	��ʼ����ʱ��3,����5ms�ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Tim1TimeIntInit(void);

#endif

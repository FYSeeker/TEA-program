#include <stdbool.h>
#include <stddef.h>
#include "control.h"
#include "encoder.h"
#include "motor.h"
#include "led.h"
#include "inv_mpu.h"
#include "key.h"
#include "delay.h"
#include "usart.h"


PID upPD = { 460, 0, 1 };			//ֱ��PD����
PID velocityPI = { 170, 0.8, 0 };	//�ٶ�PI����
PID turnP = {1.2, 0, 0};				//ת��P����

s8 LeftSteer = 0;
s8 RightSteer = 0;
s8 Steer = 0;		//ƽ�⳵��ʻ����
s16 TurnValue = 0;	//ƽ�⳵ת�����

static void GetEncoderNum(short *encoderLeftNum, short *encoderRightNum);
static short UprightPd(float pitch, short yAngularSpeed);
static short SpeedPi(short encoderLeftNum, short encoderRightNum, u8 mode);
static int TurnPID(int encoder_left,int encoder_right,float yaw);
static void LimitPwm(short *leftPwmValue, short *rightPwmValue);
static u8 CheckState(float pitch);
static void SetPwm(short leftPwmValue, short rightPwmValue);

/******************************************************************
*��������:	KeyStartAndStop
*��������:	�������ڿ����͹ر�С��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void KeyStartAndStop(void)
{
	static bool flag = 0;
	
	if (KEY == 0)
	{
		delay_ms(10);
		
		if (KEY == 0)
		{
			flag = !flag;
			while(KEY == 0);
			
			if (flag)
			{
				TIM_Cmd(TIM1, ENABLE);		//ʹ��TIM3
			}
			else
			{
				TIM_Cmd(TIM1, DISABLE);		//ʧ��TIM3
				
				//�رյ��,���
				AIN1 = 0;
				AIN2 = 0;
				BIN1 = 0;
				BIN2 = 0;
				LED = 1;
				
				//�������
				SaveAndRead(NULL, NULL, NULL, NULL, EMPTY_DATA);	//�����ʾ����				
				SpeedPi(0, 0, EMPTY_INTEGRAL);						//��ջ���
			}	
		}
	}
}

/***********************************************************************
*��������:	SaveAndRead
*��������:	���ڱ���Ͷ�ȡ��Ҫ������(������,Y����ٶ�,��߱���������,
*			�ұ߱���������),������ȫ�ֱ�����ʹ��,���ͳ�����϶�
*��������:	pitch:				ָ�����ǵ�ָ��
*			yAngularSpeed:		ָ��Y����ٶȵ�ָ��
*			encoderLeftNum:		ָ����߱�����������ָ��
*			encoderRightNum:	ָ���ұ߱�����������ָ��
*			mode:				ģʽ(SAVE_DATAΪ��������,
*								READ_DATAΪ��ȡ����,
*								EMPTY_DATAΪ�������)
*�� �� ֵ:	��
************************************************************************/
void SaveAndRead(float *pitch, short *yAngularSpeed, short *encoderLeftNum, short *encoderRightNum, u8 mode)
{
	static float savePitch = 0.0;
	static short saveYAngularSpeed = 0;
	static short saveEncoderLeftNum = 0;
	static short saveEncoderRightNum = 0;
	
	if (mode == SAVE_DATA)
	{
		savePitch = *pitch;
		saveYAngularSpeed = *yAngularSpeed;
		saveEncoderLeftNum = *encoderLeftNum;
		saveEncoderRightNum = *encoderRightNum;
	}
	else if (mode == READ_DATA)
	{
		*pitch = savePitch;
		*yAngularSpeed = saveYAngularSpeed;
		*encoderLeftNum = saveEncoderLeftNum;
		*encoderRightNum = saveEncoderRightNum;
	}
	else if (mode == EMPTY_DATA)
	{
		savePitch = 0.0;
		saveYAngularSpeed = 0;
		saveEncoderLeftNum = 0;
		saveEncoderRightNum = 0;
	}
}

/******************************************************************
*��������:	Tim3TimeIntInit
*��������:	��ʼ����ʱ��3,����5ms�ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Tim1TimeIntInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	TIM_TimeBaseStructure.TIM_Period = 4999;
	TIM_TimeBaseStructure.TIM_Prescaler = 71;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;				//�����ȼ�3��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM1, DISABLE);
}

/******************************************************************
*��������:	TIM1_UP_IRQHandler
*��������:	��ʱ��1�ĸ����ж�,5ms
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 time = 0;				//�����Ƽ�������
	static float pitch = 0.0;			//������
	static float roll = 0;				//�����
	static float yaw = 0;				//�����		
	static short yAngularSpeed = 0;		//Y����ٶ�	
	static short zAngularSpeed = 0;		//Z����ٶ�
	static short encoderLeftNum = 0;	//��߱���������
	static short encoderRightNum = 0;	//�ұ߱���������
	static short upPwmValue = 0;		//ֱ��PD���ڵ�PWM����ֵ
	static short velocityPwmValue = 0;	//�ٶ�PI���ڵ�PWM����ֵ
	static short turnPwmValue = 0;		//ת����ڵ�PWM����ֵ
	static short leftPwmValue = 0;		//��ߵ����PWMֵ
	static short rightPwmValue = 0;		//�ұߵ����PWMֵ
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ
		
		GetEncoderNum(&encoderLeftNum, &encoderRightNum);		//�õ����ұ���������ֵ								
		
		mpu_dmp_get_data(&pitch, &roll, &yaw, &yAngularSpeed, &zAngularSpeed);	//����λ��
		
//		SaveAndRead(&pitch, &yAngularSpeed, &encoderLeftNum, &encoderRightNum, SAVE_DATA);	//��������
		
		upPwmValue = UprightPd(pitch, yAngularSpeed);								//ֱ��PD����
		velocityPwmValue = SpeedPi(encoderLeftNum, encoderRightNum, NORMAL_MODE);	//�ٶ�PI����
		turnPwmValue = TurnPID(encoderLeftNum, encoderRightNum, zAngularSpeed);		//ת��P����
		
		leftPwmValue = upPwmValue + velocityPwmValue + turnPwmValue;		//��ߵ��PWM��ֵ
		rightPwmValue = upPwmValue + velocityPwmValue - turnPwmValue;		//�ұߵ��PWM��ֵ
		
		LimitPwm(&leftPwmValue, &rightPwmValue);				//�޷�
		if (CheckState(pitch) == 0)								//���״̬����
		{
			SetPwm(leftPwmValue, rightPwmValue);				//���õ����PWMֵ
		}
		
		//������ָʾ����״̬
		time++;
		if (time > 149)
		{
			time = 0;
			LED = !LED;
		}	
	}	
}

/******************************************************************
*��������:	GetEncoderNum
*��������:	�õ����ұ���������ֵ
*��������:	encoderLeftNum:		��߱���������ָ��
*			encoderRightNum:	�ұ߱���������ָ��
*�� �� ֵ:	��
*******************************************************************/
static void GetEncoderNum(short *encoderLeftNum, short *encoderRightNum)
{
	u16 leftTemp;
	u16 rightTemp;	
	
	leftTemp = ReadEncoderValue(2);			//��ȡ���������ֵ
	if (leftTemp > 32767)					//��ת,���¼���
	{
		*encoderLeftNum = leftTemp - 65535;
	}
	else
	{
		*encoderLeftNum = leftTemp;
	}
	*encoderLeftNum = -*encoderLeftNum;		//���ұ������ӿ��෴,��Ҫ�����ȡ��
	
	rightTemp = ReadEncoderValue(4);		//��ȡ�ұ�������ֵ
	if (rightTemp > 32767)
	{
		*encoderRightNum = rightTemp - 65535;
	}
	else
	{
		*encoderRightNum = rightTemp;
	}
}

/******************************************************************
*��������:	UprightPd
*��������:	ֱ��PD����
*��������:	pitch:			������
*			yAngularSpeed:	Y����ٶ�
*�� �� ֵ:	upPwmValue:		PWM����ռ�ձ�ֵ
*******************************************************************/
static short UprightPd(float pitch, short yAngularSpeed)
{
	short upPwmValue;
	float bias;		//ƫ��ֵ
	
	bias = pitch - 0;		//0��ƽ��
	
	upPwmValue = upPD.kp * bias + upPD.kd * yAngularSpeed;
	
	return upPwmValue;
}

/****************************************************************************************
*��������:	SpeedPi
*��������:	�ٶ�PI����,����ֹͣʱ,�� EMPTY_INTEGRAL ģʽ��ջ���
*��������:	encoderLeftNum:		�����������
*			encoderRightNum:	�ұ���������
*			mode:				ģʽ(NORMAL_MODEΪ��������,EMPTY_INTEGRALΪ��ջ���)
*�� �� ֵ:	velocityPwmValue:	PWM����ռ�ձ�ֵ
******************************************************************************************/
static short SpeedPi(short encoderLeftNum, short encoderRightNum, u8 mode)
{
	int velocityPwmValue;
	static float bias;		//ƫ��ֵ
	static float lastBias;	//��һ��ƫ��ֵ
	static float integral;	//����ֵ
	
	if (mode == EMPTY_INTEGRAL)
	{
		integral = 0;
		
		return 0;
	}
	
	bias = (encoderLeftNum + encoderRightNum) - 0;				//Ŀ��ֵΪ0
	bias = bias * 0.2 + lastBias * 0.8;	//һ�׵�ͨ�˲�

	integral = integral+bias+Steer;
	
	//�����޷�
	if (integral > MAX_INTEGRAL)
	{
		integral = MAX_INTEGRAL;
	}
	else if (integral < -MAX_INTEGRAL)
	{
		integral = -MAX_INTEGRAL;
	}
	
	velocityPwmValue = velocityPI.kp * bias + velocityPI.ki * integral;
	
	//PI�ٶȻ��޷�
	if (velocityPwmValue > MAX_PI)
	{
		velocityPwmValue = MAX_PI;
	}
	else if (velocityPwmValue < -MAX_PI)
	{
		velocityPwmValue = -MAX_PI;
	}
	
	lastBias = bias;	//��¼�ϴ����
	
	return (short)velocityPwmValue;
}

/**************************************************************************
�������ܣ�ת�����  
��ڲ��������ֱ����������ֱ�������Z��������
����  ֵ��ת�����PWM
**************************************************************************/
static int TurnPID(int encoder_left,int encoder_right,float zAngularSpeed)//ת�����
{
	float value;
	static float bias;
	
	bias = zAngularSpeed - TurnValue;
	
	value = turnP.kp*bias;
	
	return (int)value;
}

/******************************************************************
*��������:	LimitPwm
*��������:	����PWM���ķ�ֵ
*��������:	leftPwmValue:	��ߵ����PWMֵ��ָ��
*			rightPwmValue:	�ұߵ����PWMֵ��ָ��
*�� �� ֵ:	��
*******************************************************************/
static void LimitPwm(short *leftPwmValue, short *rightPwmValue)
{
	if (*leftPwmValue > PWM_MAX_VALUE)
	{
		*leftPwmValue = PWM_MAX_VALUE;
	}
	else if (*leftPwmValue < -PWM_MAX_VALUE)
	{
		*leftPwmValue = -PWM_MAX_VALUE;
	}
	
	if (*rightPwmValue > PWM_MAX_VALUE)
	{
		*rightPwmValue = PWM_MAX_VALUE;
	}
	else if (*rightPwmValue < -PWM_MAX_VALUE)
	{
		*rightPwmValue = -PWM_MAX_VALUE;
	}
}

/******************************************************************
*��������:	CheckState
*��������:	���С����״̬,��������Ǿ���ֵ����40��,ֹͣ���
*��������:	pitch:	������
*�� �� ֵ:	0:		״̬����
*			����:	״̬�쳣
*******************************************************************/
static u8 CheckState(float pitch)
{
	if ((pitch > 40) || (pitch < -40))	//�����Ǿ���ֵ����40��
	{
		//ֹͣ���
		AIN1 = 0;
		AIN2 = 0;
		BIN1 = 0;
		BIN2 = 0;
		
		SpeedPi(0, 0, EMPTY_INTEGRAL);	//��ջ���
		
		return 1;
	}
	
	return 0;
}

/******************************************************************
*��������:	SetPwm
*��������:	�������ҵ����PWM��
*��������:	leftPwmValue:	��ߵ����PWMֵ
*			rightPwmValue:	�ұߵ����PWMֵ
*�� �� ֵ:	��
*******************************************************************/
static void SetPwm(short leftPwmValue, short rightPwmValue)
{
	if (leftPwmValue < 0)
	{
		AIN1 = 1;
		AIN2 = 0;
		
		PWMA = -leftPwmValue;
	}
	else
	{
		AIN1 = 0;
		AIN2 = 1;
		
		PWMA = leftPwmValue;
	}
	
	if (rightPwmValue < 0)
	{
		BIN1 = 1;
		BIN2 = 0;
		
		PWMB = -rightPwmValue;
	}
	else
	{
		BIN1 = 0;
		BIN2 = 1;
		
		PWMB = rightPwmValue;
	}
}

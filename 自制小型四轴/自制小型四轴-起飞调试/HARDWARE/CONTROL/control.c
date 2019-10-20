#include "control.h"
#include "motor.h"
#include "led.h"
#include "inv_mpu.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "24l01.h"


PID Pid_X = {12, 0, 0.2};
PID Pid_Y = {12, 0, 0.2};
PID Pid_Z = {6, 0, 0};


u8 LOCK_FLAG = 0;		//������־
u16 Accelerator = 1000;	//����ֵ

/******************************************************************
*��������:	CheckState
*��������:	��������״̬,��������Ǿ���ֵ����40��,ֹͣ���
*��������:	pitch:	������;roll: �����
*�� �� ֵ:	0:		״̬����
*			����:	״̬�쳣
*******************************************************************/
u8 CheckState(float pitch, float roll)
{
	if (((pitch > 45) || (pitch < -45)) || ((roll > 45) || (roll < -45)))	//����ֵ����45��
	{
		return 1;
	}
	
	return 0;
}


/******************************************************************
*��������:	PID_Control
*��������:	PID���ƺ���
*��������:	MPU6050����
*�� �� ֵ:	��
*******************************************************************/
void PID_Control(MPUDATA MpuData, MPUDATA MpuAS)
{
	XYZ_DATA OUT;
	static XYZ_DATA Error;			//���ֵ��������
	static XYZ_DATA Error_Add;		//�ۼ����ֵ�����֣�
	
	/*���㱾�νǶ����*/
	Error.X = MpuData.pitch;
	Error.Y = MpuData.roll;
	Error.Z = MpuData.yaw;
	
	/*����ǶȻ���*/
	Error_Add.X += Error.X;
	Error_Add.Y += Error.Y;
	Error_Add.Z += Error.Z;
	
	/*X��ǶȻ����޷�*/
	if(Error_Add.X > MAX_INTEGRAL)		 Error_Add.X = MAX_INTEGRAL;
	else if(Error_Add.X < -MAX_INTEGRAL) Error_Add.X = -MAX_INTEGRAL;

	/*Y��ǶȻ����޷�*/
	if(Error_Add.X > MAX_INTEGRAL)		 Error_Add.X = MAX_INTEGRAL;
	else if(Error_Add.X < -MAX_INTEGRAL) Error_Add.X = -MAX_INTEGRAL;
	
	/*PID����*/
	OUT.X = Pid_X.P*Error.X + Pid_X.I*Error_Add.X + Pid_X.D*MpuAS.X_AngularSpeed;
	OUT.Y = Pid_Y.P*Error.Y + Pid_Y.I*Error_Add.Y + Pid_Y.D*MpuAS.Y_AngularSpeed;
	OUT.Z = Pid_Z.P*MpuAS.Z_AngularSpeed;
	
	/*PWM����޷�*/
	if(OUT.X > X_PWM_MAX)		OUT.X = X_PWM_MAX;
	else if(OUT.X < -X_PWM_MAX)	OUT.X = -X_PWM_MAX;
	
	if(OUT.Y > Y_PWM_MAX)		OUT.Y = Y_PWM_MAX;
	else if(OUT.Y < -Y_PWM_MAX)	OUT.Y = -Y_PWM_MAX;
	
	if(OUT.Z > Z_PWM_MAX)		OUT.Z = Z_PWM_MAX;
	else if(OUT.Z < -Z_PWM_MAX)	OUT.Z = -Z_PWM_MAX;
	
	/*���õ��PWM��ֵ*/
	PWM1 = Accelerator + OUT.X - OUT.Y - OUT.Z;
	PWM2 = Accelerator + OUT.X + OUT.Y + OUT.Z;
	PWM3 = Accelerator - OUT.X - OUT.Y + OUT.Z;
	PWM4 = Accelerator - OUT.X + OUT.Y - OUT.Z;
}


/******************************************************************
*��������:	Descend
*��������:	�����½���������ˤ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Descend()
{
	u8 t;
	
	TIM_Cmd(TIM1,DISABLE);	//�رն�ʱ��
	
	PWM1 = THRESHOLD;
	PWM2 = THRESHOLD;
	PWM3 = THRESHOLD;
	PWM4 = THRESHOLD;
	
	for(t=0;t<10;t++)	//�½�ʱ��10s
	{
		delay_ms(1000);
	}

	PWM1 = 0;
	PWM2 = 0;
	PWM3 = 0;
	PWM4 = 0;
}


/******************************************************************
*��������:	TIM1_IntInit
*��������:	��ʼ����ʱ��1,�����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_IntInit(u16 arr,u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

	/*��ʼ����ʱ������*/
	TIM_TimeBaseStructure.TIM_Period = arr;
	TIM_TimeBaseStructure.TIM_Prescaler = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;		//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);					//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

	/*��ʼ����ʱ���ж�*/
	TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);						//��ʱ�������ж�
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;		//��ռ���ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;				//�����ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM1, ENABLE);
}

/******************************************************************
*��������:	TIM1_UP_IRQHandler
*��������:	��ʱ��1�ĸ����ж�,5ms
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	static u16 time = 0;					//ָʾ�Ƽ�������
	static MPUDATA MpuData = {0,0,0};		//ŷ����
	static MPUDATA MpuAS = {0,0,0};			//���ٶ�
	
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ					
			
		/*PID����*/
		if((LOCK_FLAG == 1)&&(Accelerator>1500))		//����״̬
		{
			if(mpu_dmp_get_data(&MpuData.pitch,&MpuData.roll,&MpuData.yaw,
				&MpuAS.X_AngularSpeed,&MpuAS.Y_AngularSpeed,&MpuAS.Z_AngularSpeed)==0)	//�ɹ���ȡ����
			{		
				if(CheckState(MpuData.pitch, MpuData.roll)==0)	//�ж��Ƿ������ͻ���״̬
				{
					PID_Control(MpuData, MpuAS);
				}
				else
				{
					PWM1 = 0;
					PWM2 = 0;
					PWM3 = 0;
					PWM4 = 0;
				}			
			}
		}
		else		//����״̬
		{
			PWM1 = 0;
			PWM2 = 0;
			PWM3 = 0;
			PWM4 = 0;
		}
		
		//������ָʾ����״̬
		time++;
		
		if(time%100==0)
		{
			LED0 = !LED0;
		}
		
		/*��ʧ������*/
		if(time > 599)				//3sʱ��
		{
			if(Nrf_Buff[4] == 1)	//δʧ��
			{			
				Nrf_Buff[4] = 0;	//������ӱ�־
				LED1 = 1;
				printf("OK\r\n");
			}
			else					//����ʧ��
			{
				Nrf_Buff[3] = 0;	//������ӱ�־
				LOCK_FLAG = 0;		//��������
				LED1 = 0;			//�����
				printf("NO\r\n");
			}
			time = 0;
		}
	}
}


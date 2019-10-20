#include "timer.h"
#include "motor.h"
#include "encoder.h"
#include "lcd.h"
#include "dcmi.h"
#include "EasyTracer.h"

u8 Run=0;

_PID Pid = {0,0,0};		//170,0,1.5

/******************************************************************
*��������:	PID���ڣ�λ��ʽ
*��������:	position��ʵ��λ��
			mode������ģʽ
*�� �� ֵ:	PWM��
*******************************************************************/
short PID_Adjust(u16* position,u8 mode)
{
	float value=0;
	static float Error=0;				//��ǰ���
	static float LastError=0;			//�ϴ����
	static float Integral=0;			//������
	
	LastError = Error;					//��¼�ϴ����
	Error = *position - 160;			//���㱾�����
	
	if(mode) Integral += Error;
	else Integral = 0;
	
	/*�����޷�*/
	if(Integral>Integral_MAX) Integral = Integral_MAX;
	else if(Integral<-Integral_MAX) Integral = -Integral_MAX;
	
	value = Pid.Kp * Error + Pid.Ki * Integral + Pid.Kd * (Error - LastError);
	
	/*����޷�*/
	if(value>PWM_MAX)
	{
		value = PWM_MAX;
	}
	else if(value<-PWM_MAX)
	{
		value = -PWM_MAX;
	}
	
	return (short)value;
}


/******************************************************************
*��������:	PWM������
*��������:	pwmvalue��PWM��ֵ
			mode������ģʽ
*�� �� ֵ:	��
*******************************************************************/
void PWM_Control(short pwmvalue)
{
	u8 mode=0;

	/*�����ж�*/
	if(pwmvalue<0)
	{
		pwmvalue = -pwmvalue;
		mode = RIGHT;	//��ת
	}
	else
	{
		mode = LEFT;	//��ת
	}
	Motor_Control(pwmvalue,pwmvalue,mode);
}


/**************************************************************************************************************/

TIM_HandleTypeDef TIM1_Handler;      //��ʱ����� 

/***************************************************************************
�������ܣ�	��ʱ���жϳ�ʼ��
��ڲ�����	arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
���ز�����	��
����˵��;	��ʱ�����ʱ����㷽��:Tout=((arr+1)*(psc+1))/Ft us.
			Ft=��ʱ������Ƶ��,��λ:Mhz
			��ʱ��1����APB2�ϣ�ʱ��ΪAPB2*2=180MHz
****************************************************************************/
void TIM1_Init(u16 arr,u16 psc)
{  
    TIM1_Handler.Instance=TIM1;                          //��ʱ��1
    TIM1_Handler.Init.Prescaler=psc;                     //��Ƶ
    TIM1_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;    //���ϼ�����
    TIM1_Handler.Init.Period=arr;                        //�Զ�װ��ֵ
    TIM1_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_Base_Init(&TIM1_Handler);
    
    HAL_TIM_Base_Start_IT(&TIM1_Handler);	//ʹ�ܶ�ʱ���Ͷ�ʱ���ж�
}

//��ʱ���ײ�����������ʱ�ӣ������ж����ȼ�
//�˺����ᱻHAL_TIM_Base_Init()��������
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	__HAL_RCC_TIM1_CLK_ENABLE();					//ʹ��TIM3ʱ��
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn,1,3);	//�����ж����ȼ�����ռ���ȼ�1�������ȼ�3
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);			//����ITM1�����ж� 
}

//��ʱ��1�жϷ�����
void TIM1_UP_TIM10_IRQHandler(void)
{
    HAL_TIM_IRQHandler(&TIM1_Handler);
}


/*���߱�־*/
u8 Center_flag=0;
u16 MidlinePos = 160;

//��ʱ��3�жϷ���������
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
//	static short PwmValue=0;
	static u8 mode=STOP;
//	static u8 TimCount=0;
	
    if(htim==(&TIM1_Handler))
    {	
		/*��ӡ֡��*/
//		TimCount++;
//		if(TimCount>9)
//		{
//			TimCount = 0;
//			printf("%d\t",ov_frame);
//			ov_frame = 0;
//		}
		
		if(Run)					//������
		{
			if(Center_flag)		//ʶ������
			{
				Center_flag = 0;
				
//				PwmValue = PID_Adjust(&MidlinePos,0);
				
				if(MidlinePos>170)
				{
					mode = LEFT;
				}
				else if(MidlinePos<150)
				{
					mode = RIGHT;
				}
				else
				{
					mode = FRONT;
				}
			}
			else
			{
				mode = STOP;
			}
			Motor_Control(1600,1600,mode);
		}
		else
		{
			Motor_Control(0,0,STOP);	//ͣ��
		}
    }
}



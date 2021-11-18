#include "control.h"
#include "pwm.h"
#include "delay.h"
#include "mytouch.h"
#include "usart.h"
#include "adc.h"

extern u16 AdcValue;

u8 Run = 0;
u16 IntTime = 0;
short Aim = 1050;
float Angle = 0;

PID upPid = {14, 0, 35};		//21,108
PID SpeedPid = {336, 0.3, 0};	//650,0


/**********************************************************************
�������ܣ�����ADC��ֵ����Ƕ�
��ڲ�����ADC��ֵ
���ز������Ƕ�
***********************************************************************/
float Count_Angle(u16 value)
{
	float angle;
	
	angle = (360.0/4095)*value;
	
	if(angle>275) angle = angle-275;
	else angle = angle+(360-275);
	
	return angle;
}


void Procedure_1()
{
	if(IntTime<59)
	{
		PWM1 = 2000;
		PWM2 = 0;
	}
	else
	{
		PWM1 = 0;
		PWM2 = 2000;
	}
	if(IntTime>119) IntTime = 0;
	IntTime++;
}


void Procedure_2()
{
	PWM1 = 7000;
	PWM2 = 0;
	delay_ms(200);
	PWM1 = 0;
	PWM2 = 4000;
	delay_ms(200);
	PWM1 = 0;
	PWM2 = 0;
	
	Run = 0;
}


short Get_EncoderValue()
{
	u16 Value;
	short EncoderValue;
	
	Value = TIM4->CNT;	
	TIM4->CNT = 0;
	
	if(Value>32767)
	{
		EncoderValue = Value - 65535;
	}
	else
	{
		EncoderValue = Value;
	}
	
	return -EncoderValue;
}


short Upright_PID(short Aim,u16* Actual,u8 Mode)
{
	short PwmValue;
	static short Bias;		//���
	static short LastBias;	//�ϴ����
	static short Integral;	//������
	
	LastBias = Bias;		//��¼�ϴ����
	Bias = Aim - *Actual;
	
	if(Mode==0) Integral += Bias;		//��������
	else Integral = 0;					//��ջ���
	
	/*�����޷�*/
	if (Integral > MAX_INTEGRAL)
	{
		Integral = MAX_INTEGRAL;
	}
	else if (Integral < -MAX_INTEGRAL)
	{
		Integral = -MAX_INTEGRAL;
	}
	
	PwmValue = upPid.Kp * Bias + upPid.Ki * Integral + upPid.Kd * (Bias - LastBias);
	
//	printf("%d\t%d\t%d\t%d\t%d\r\n",AdcValue,*Actual,Bias,LastBias,Bias - LastBias);
	
	/*PWM�޷�*/
	if (PwmValue > PWM_MAX_VALUE)
	{
		PwmValue = PWM_MAX_VALUE;
	}
	else if (PwmValue < -PWM_MAX_VALUE)
	{
		PwmValue = -PWM_MAX_VALUE;
	}
	
	return (short)PwmValue;
}


short Speed_PID(short EncoderValue,u8 Mode)
{
	int PwmValue;
	static float Bias;		//���
	static float LastBias;	//�ϴ����
	static float Integral;	//������
	
	LastBias = Bias;		//��¼�ϴ����
	Bias = EncoderValue;
	Bias = Bias * 0.3 + LastBias * 0.7;
	
	if(Mode==0) Integral += Bias;		//��������
	else Integral = 0;					//��ջ���
	
	//�����޷�
	if (Integral > MAX_INTEGRAL)
	{
		Integral = MAX_INTEGRAL;
	}
	else if (Integral < -MAX_INTEGRAL)
	{
		Integral = -MAX_INTEGRAL;
	}
	
	PwmValue = SpeedPid.Kp * Bias + SpeedPid.Ki * Integral + SpeedPid.Kd * (Bias - LastBias);
	
	/*PWM�޷�*/
	if (PwmValue > PWM_MAX_VALUE)
	{
		PwmValue = PWM_MAX_VALUE;
	}
	else if (PwmValue < -PWM_MAX_VALUE)
	{
		PwmValue = -PWM_MAX_VALUE;
	}
	
	return (short)PwmValue;
}


void Procedure_3(void)
{
	short PwmValue = 0;
	static short TurnPwm = 1000;
	
	if((AdcValue>400) && (AdcValue<1700))
	{
		PwmValue = Upright_PID(Aim,&AdcValue,1);
		PwmValue += Speed_PID(Get_EncoderValue(),0);
		
		if(Key_Flag==6)
		{
			PwmValue += TurnPwm;
		}
		
		/*�����ж�*/
		if(PwmValue<0)
		{
			PWM1 = -PwmValue;
			PWM2 = 0;
		}
		else
		{		
			PWM1 = 0;
			PWM2 = PwmValue;
		}
	}
	else
	{	
		Speed_PID(Get_EncoderValue(),1);
		
		if(Key_Flag==4)
		{
			Procedure_1();
		}
		else
		{
			PWM1 = 0;
			PWM2 = 0;
		}
	}
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
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;		//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;				//�����ȼ�0��
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
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ					
					
		if(Run)
		{
			switch(Key_Flag)
			{
				case 1:Procedure_1();			
					break;
				
				case 3:
				case 4:
				case 5:
				case 6:Procedure_3();
					break;
			}	
		}
		else
		{
			PWM1 = 0;
			PWM2 = 0;
		}
	}
}



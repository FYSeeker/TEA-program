#include "encoder.h"

TIM_HandleTypeDef TIM3_Handler;
TIM_Encoder_InitTypeDef TIM3_Encoder_Init;

/***************************************************************************
�������ܣ�	��ʱ��3������ģʽ��ʼ��
��ڲ�����	��
���ز�����	��
****************************************************************************/
void TIM3_EncoderInit(void)
{  
    TIM3_Handler.Instance=TIM3;								//��ʱ��3
    TIM3_Handler.Init.Prescaler=0;							//��Ƶ
    TIM3_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;		//���ϼ�����
    TIM3_Handler.Init.Period=65535;							//�Զ�װ��ֵ
    TIM3_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
	
	TIM3_Encoder_Init.EncoderMode = TIM_ENCODERMODE_TI12;
	TIM3_Encoder_Init.IC1Filter = 0;
	TIM3_Encoder_Init.IC1Prescaler = 0;
	TIM3_Encoder_Init.IC1Polarity = TIM_ICPOLARITY_RISING;
	TIM3_Encoder_Init.IC1Selection = TIM_ICSELECTION_DIRECTTI;
	TIM3_Encoder_Init.IC2Filter = 0;
	TIM3_Encoder_Init.IC2Prescaler = 0;
	TIM3_Encoder_Init.IC2Polarity = TIM_ICPOLARITY_RISING;
	TIM3_Encoder_Init.IC2Selection = TIM_ICSELECTION_DIRECTTI;
	
	HAL_TIM_Encoder_Init(&TIM3_Handler,&TIM3_Encoder_Init);
	
	HAL_TIM_Encoder_Start(&TIM3_Handler,TIM_CHANNEL_1);
	HAL_TIM_Encoder_Start(&TIM3_Handler,TIM_CHANNEL_2);
}



/***************************************************************************
�������ܣ�	��ʱ���ײ�������ʱ��ʹ�ܣ���������
��ڲ�����	htim����ʱ����������ʼ�����
���ز�����	��
����˵��;	�˺����ᱻHAL_TIM_Encoder_Init()����
****************************************************************************/
void HAL_TIM_Encoder_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM3_CLK_ENABLE();			//ʹ�ܶ�ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_6 | GPIO_PIN_7;	//pin
    GPIO_Initure.Mode=GPIO_MODE_INPUT;			//��������
    GPIO_Initure.Pull=GPIO_NOPULL;				//��
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//����
	GPIO_Initure.Alternate= GPIO_AF2_TIM3;		//����ΪTIM
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}


/******************************************************************
*��������:	Tim2EncoderInit
*��������:	��ʼ����ʱ��2,���ڱ�����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
/*
void Tim2EncoderInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ�ܶ�ʱ��4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��PB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;	//��������
	GPIO_Init(GPIOA, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					//Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���  
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM2, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);			//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM2, 0);
	
	TIM_Cmd(TIM2, ENABLE);
}
*/

/******************************************************************
*��������:	Tim4EncoderInit
*��������:	��ʼ����ʱ��4,���ڱ�����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
/*
void Tim4EncoderInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//ʹ�ܶ�ʱ��4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//ʹ��PB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//�����趨������ʼ��GPIOB

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0x0;					// Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Period = ENCODER_TIM_PERIOD;		//�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);	//ʹ�ñ�����ģʽ3
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);			//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM4, 0);
	
	TIM_Cmd(TIM4, ENABLE);
}
*/

/******************************************************************
*��������:	ReadEncoderValue
*��������:	��ȡ��������ֵ
*��������:	timeValue:		��ʱ����(3��5)
*�� �� ֵ:	encoderValue:	��ȡָ���Ķ�ʱ����CNTֵ
*******************************************************************/
u16 ReadEncoderValue(u8 timeValue)
{
	u16 encoderValue;
	
	switch (timeValue)
	{
		case 3:
			encoderValue = TIM3->CNT;
			TIM3->CNT = 0;
			break;
		case 5:
			encoderValue = TIM5->CNT;
			TIM5->CNT = 0;
			break;
		default:
			encoderValue = 0;
			break;
	}
	
	return encoderValue;
}


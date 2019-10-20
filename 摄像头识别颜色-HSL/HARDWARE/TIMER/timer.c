#include "timer.h"


/******************************************************************
*��������: TIM3_PWM_Init
*��������: ��ʼ����ʱ��,PA6 PA7����PWM��
*��������: arr:�Զ���װֵ
		   psc:�Զ�Ԥ��Ƶ��
*�� �� ֵ: ��
*******************************************************************/
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/*PWM�����ʱ��ʹ��*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);		//ʹ��GPIO
	
   /*PWM��IO�ڳ�ʼ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//CH1,CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	/*PWM����ʱ��������ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;					//����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);				//��ʼ��TIMx
	
	/*��ʱ��PWM�������ʼ��*/ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;			//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;							//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);					//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);					//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR2�ϵ�Ԥװ�ؼĴ���
	
	TIM_ARRPreloadConfig(TIM3, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	TIM_Cmd(TIM3, ENABLE);	//ʹ�ܶ�ʱ��
}


/******************************************************************
*��������: TIM8_PWM_Init
*��������: ��ʼ����ʱ��8, PC8����PWM��
*��������: arr:�Զ���װֵ
		   psc:�Զ�Ԥ��Ƶ��
*�� �� ֵ: ��
*******************************************************************/
void TIM8_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	
	/*PWM�����ʱ��ʹ��*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM8, ENABLE);
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);		//ʹ��GPIO
	
   /*PWM��IO�ڳ�ʼ��*/
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;					//CH3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;				//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	/*PWM����ʱ��������ʼ��*/
	TIM_TimeBaseStructure.TIM_Period = arr;						//�����Զ���װ������ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc;					//����Ԥ��Ƶֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;				//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM8, &TIM_TimeBaseStructure);				//��ʼ��TIMx
	
	/*��ʱ��PWM�������ʼ��*/ 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;			//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ1
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0;							//���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;	//�������:TIM����Ƚϼ��Ը�
	
	TIM_OC3Init(TIM8, &TIM_OCInitStructure);					//����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	
	TIM_CtrlPWMOutputs(TIM8, ENABLE);	//MOE �����ʹ��,�߼���ʱ�����뿪�����
	TIM_OC3PreloadConfig(TIM8, TIM_OCPreload_Enable);  //ʹ��TIMx��CCR3�ϵ�Ԥװ�ؼĴ���
	TIM_ARRPreloadConfig(TIM8, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ��� 
	TIM_Cmd(TIM8, ENABLE);				//ʹ�ܶ�ʱ��

	TIM_SetCompare3(TIM8,1000);
}


/******************************************************************
*��������:	Tim4EncoderInit
*��������:	��ʼ����ʱ��4, PB6 PB7���ڱ�����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Tim4EncoderInit(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);		//ʹ�ܶ�ʱ��4��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//ʹ��PB�˿�ʱ��

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;		//CH1(PB6),CH2(PB7)�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;		//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//�����趨������ʼ��GPIOB

	TIM_TimeBaseStructInit(&TIM_TimeBaseStructure);
	TIM_TimeBaseStructure.TIM_Prescaler = 0;					// Ԥ��Ƶ�� 
	TIM_TimeBaseStructure.TIM_Period = 65535;					//�趨�������Զ���װֵ
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;		//ѡ��ʱ�ӷ�Ƶ������Ƶ
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	//TIM���ϼ���  
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
	
	TIM_EncoderInterfaceConfig(TIM4, TIM_EncoderMode_TI2, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising);//ʹ�ñ�����ģʽ2
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	TIM_ICInitStructure.TIM_ICFilter = 10;
	TIM_ICInit(TIM4, &TIM_ICInitStructure);
	
	TIM_ClearFlag(TIM4, TIM_FLAG_Update);			//���TIM�ĸ��±�־λ
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
	TIM_SetCounter(TIM4, 0);						//��ʼ��CNT�еļ���ֵΪ0
	
	TIM_Cmd(TIM4, ENABLE);
}


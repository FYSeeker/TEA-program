#include "pwm.h"

void Balance_PWM_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB1ENR|=1<<2;       //TIM4ʱ��ʹ��    
	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ��     
	GPIOB->CRL&=0X0FFFFFFF;   //PORTB 7�������
	GPIOB->CRL|=0XB0000000;   //PORTB 7�������   
	GPIOB->CRH&=0XFFFF00F0;   //PORTB 8�������,10,11�������
	GPIOB->CRH|=0X0000220B;   //PORTB 8�������,10,11�������
	
	TIM4->ARR=arr;//�趨�������Զ���װֵ 
	TIM4->PSC=psc;//Ԥ��Ƶ������Ƶ
	
	TIM4->CCMR1|=6<<12;	//CH2 PWM2ģʽ		
	TIM4->CCMR1|=1<<11;	//CH2Ԥװ��ʹ��	  
	TIM4->CCER|=1<<4; 	//CH2���ʹ��	
	TIM4->CCMR2|=6<<4;	//CH3 PWM2ģʽ		
	TIM4->CCMR2|=1<<3;	//CH3Ԥװ��ʹ��	  
	TIM4->CCER|=1<<8; 	//CH3���ʹ��
  	
	TIM4->CR1=0x0080;  	//ARPEʹ�� 					///////////��仰�е��ɻ󣬲�ȷ���Բ���
	TIM4->CR1|=0x01;   	//ʹ�ܶ�ʱ��3 

	
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

/*********************************************************
*����������IO�����ú���
*���룺
*���ܣ�
*�����
**********************************************************/
void MOTO_DIR_Init(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	 //ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin= GPIO_Pin_10|GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed= GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_10);
	GPIO_SetBits(GPIOB, GPIO_Pin_11);
}

/*********************************************************
*������PWM�����ʼ��, ��ʱ��3 CH1��CH2��pwm���
*���룺�Զ���װ��������Ƶ��//arr���Զ���װֵ, psc��ʱ��Ԥ��Ƶ
*���ܣ���ʱ��2ͨ��1��2����Ϊ������ģʽ
*���: ��
**********************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);// 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA , ENABLE);  //ʹ��GPIO����ʱ��ʹ��

   //���ø�����Ϊ�����������,���TIM1 CH1��PWM���岨��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1; //TIM_CH1, TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	
	TIM_TimeBaseStructure.TIM_Period = arr; //��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ	 80K
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ  ����Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ

 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_Pulse = 0; //���ô�װ�벶��ȽϼĴ���������ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //�������:TIM����Ƚϼ��Ը�
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);  //����TIM_OCInitStruct��ָ���Ĳ�����ʼ������TIMx

	TIM_CtrlPWMOutputs(TIM2,ENABLE);	//MOE �����ʹ��	

	TIM_OC1PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	TIM_OC2PreloadConfig(TIM2, TIM_OCPreload_Enable);  //CH1Ԥװ��ʹ��
	
	TIM_ARRPreloadConfig(TIM2, ENABLE); //ʹ��TIMx��ARR�ϵ�Ԥװ�ؼĴ���
	
	TIM2->CCR1 =  0;
	TIM2->CCR2 =  0;
	
	TIM_Cmd(TIM2, ENABLE);  //ʹ��TIM2 
   
}


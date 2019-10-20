#include "timer.h"
#include "led.h"


//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM3_PWM_Init(u16 arr,u16 psc)
{  
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  TIM_OCInitStructure;
	

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ�ܶ�ʱ��3ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��GPIO����
 
   //���ø�����Ϊ�����������,���TIM3 CH1��PWM���岨��	GPIOA.6
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;			//TIM3_CH1
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;		//�����������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);				//��ʼ��GPIO
 
   //��ʼ��TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; 						//��������һ�������¼�װ�����Զ���װ�ؼĴ������ڵ�ֵ
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 						//����������ΪTIMxʱ��Ƶ�ʳ�����Ԥ��Ƶֵ 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; 					//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  	//TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 				//����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM3 Channel2 PWMģʽ	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; 				//ѡ��ʱ��ģʽ:TIM�����ȵ���ģʽ2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; 	//�Ƚ����ʹ��
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low; 		//�������:TIM����Ƚϼ��Ե�
	TIM_OC1Init(TIM3, &TIM_OCInitStructure);  						//����Tָ���Ĳ�����ʼ������TIM3 OC1

	TIM_OC1PreloadConfig(TIM3, TIM_OCPreload_Enable); 			 	//ʹ��TIM3��CCR1�ϵ�Ԥװ�ؼĴ���
 
	TIM_Cmd(TIM3, ENABLE);  //ʹ��TIM3
	

}

/*********************************************************************
�������ƣ�TIM2_Capture_Init
�������ܣ�TIM2��ʱ����������
���������arr:�Զ���װ��ֵ
		  psc:Ԥ��Ƶϵ��
���ز�������
*********************************************************************/


void TIM2_Capture_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//��ʱ����ʼ���ṹ��
 	NVIC_InitTypeDef NVIC_InitStructure;				//�жϳ�ʼ���ṹ��
	TIM_ICInitTypeDef TIM2_ICInitStructure;				//TIM2���벶��ṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
	
	/*��ʼ����ʱ��3*/	 
	TIM_TimeBaseStructure.TIM_Period = arr; 					//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//��ʼ��TIM2
  
	/*��ʼ��TIM2���벶�����*/
	TIM2_ICInitStructure.TIM_Channel = TIM_Channel_2; 					//CC1S=01,IC1ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; 	//ӳ�䵽TI1��
  	TIM2_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;		//�����ز���
  	TIM2_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;	 			//���������Ƶ,����Ƶ 
  	TIM2_ICInitStructure.TIM_ICFilter = 0x00;							//IC1F=0000 ���������˲��� ���˲�
  	TIM_ICInit(TIM2, &TIM2_ICInitStructure);
	
	/*�жϷ����ʼ��*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1; 	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							//��ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM2,TIM_IT_Update|TIM_IT_CC2,ENABLE);			//��������ж� ,����CC2IE�����ж�	
	
//	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
}



/*********************************************************************
�������ƣ�TIM2_IRQHandler
�������ܣ�TIM2��ʱ���жϷ�����
�����������
���ز�������
*********************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2, TIM_IT_CC2)!=RESET && (nvic_flog==0))	//�����ж��Ҳ���������,��ʱģ�鷢��������
	{
		TIM_SetCounter(TIM2, 0);		//���ö�ʱ������
		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Falling);		//�����½��ز���
		nvic_flog = 1;		//��ǲ��񵽸ߵ�ƽ
	
	}
	
	else if(TIM_GetFlagStatus(TIM2, TIM_IT_CC2)!=RESET && (nvic_flog==1))	//�����ж��Ҳ����½��أ���ʱģ����յ�������
	{		
		TIM2_Number = TIM_GetCapture2(TIM2);	//��ȡ��ʱ����ֵ
		TIM_OC2PolarityConfig(TIM2,TIM_ICPolarity_Rising);		//���������ز���		
		nvic_flog = 2;		//���һ�β������
	}
	
	else if(TIM_GetFlagStatus(TIM2, TIM_IT_Update)!=RESET)	//�ж��ж�Ϊ�����жϣ���ʱģ��û�н��յ����������������10m
	{
		nvic_flog = 3;		//�������ж�
	}	
	
	TIM_ClearITPendingBit(TIM2, TIM_IT_CC2|TIM_IT_Update);		//���TIM2�жϱ�־
}


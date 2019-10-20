#include "encoder.h"

/*********************************************************
*���������������ú���
*���룺�Զ���װ��������Ƶ��
*���ܣ���ʱ��2ͨ��1��2����Ϊ������ģʽ
*�����
**********************************************************/

TIM_ICInitTypeDef  TIM2_ICInitStructure;

void Encoder_L_Init(u16 arr,u16 psc)
{	 
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0| GPIO_Pin_1;  //PA0 ���֮ǰ����  
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA0 ����  
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	GPIO_ResetBits(GPIOA,GPIO_Pin_1| GPIO_Pin_0);						 //PA0 ����
	
	//��ʼ����ʱ��2 TIM2	 
	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
	
	//��ʼ��TIM2ͨ��CH1��CH2���뼫�Ժ�ӳ��
	TIM2->CCMR1 |= 1<<0; //CC1S='01' IC1FP1ӳ�䵽TI1
	TIM2->CCMR1 |= 1<<8; //CC2S='01' IC2FP2ӳ�䵽TI2
	TIM2->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1�����࣬IC1FP1=TI1
	TIM2->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2�����࣬IC2FP2=TI2
	
	//��ʼ��������
	TIM_EncoderInterfaceConfig(TIM2, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3 �������ض�����
    TIM_ICStructInit(&TIM2_ICInitStructure);//���ṹ���е�����ȱʡ����
    TIM2_ICInitStructure.TIM_ICFilter = 6;  //ѡ�����벶���˲���
    TIM_ICInit(TIM2, &TIM2_ICInitStructure);//��TIM_ICInitStructure��ָ��������ʼ��TIM2
	
	//�жϷ����ʼ��
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  //TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
	TIM2->CNT = 0;
	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
 
}
/*********************************************************
*���������������ú���
*���룺�Զ���װ��������Ƶ��
*���ܣ���ʱ��4ͨ��1��2����Ϊ������ģʽ
*�����
**********************************************************/
//void Encoder_R_Init(u16 arr,u16 psc)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
// 	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);	//ʹ��TIM3ʱ��
// 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);  //ʹ��GPIOAʱ��
//	
//	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_6| GPIO_Pin_7;  //PA0 ���֮ǰ����  
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD; //PA6 ����  
//	GPIO_Init(GPIOA, &GPIO_InitStructure);
//	GPIO_ResetBits(GPIOA,GPIO_Pin_6| GPIO_Pin_7);						 //PA6 ����
//	

//	//��ʼ����ʱ��4 TIM4	 
//	TIM_TimeBaseStructure.TIM_Period = arr; //�趨�������Զ���װֵ 
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; 	//Ԥ��Ƶ��   
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; //����ʱ�ӷָ�:TDTS = Tck_tim
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM���ϼ���ģʽ
//	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //����TIM_TimeBaseInitStruct��ָ���Ĳ�����ʼ��TIMx��ʱ�������λ
//	
//	//��ʼ��TIM4ͨ��CH1��CH2���뼫�Ժ�ӳ��
//	TIM3->CCMR1 |= 1<<0; //CC1S='01' IC1FP1ӳ�䵽TI1
//	TIM3->CCMR1 |= 1<<8; //CC2S='01' IC2FP2ӳ�䵽TI2
//	TIM3->CCER &= ~(1<<1);	 //CC1P='0'	 IC1FP1�����࣬IC1FP1=TI1
//	TIM3->CCER &= ~(1<<5);	 //CC2P='0'	 IC2FP2�����࣬IC2FP2=TI2
//	
//	//��ʼ��������
//	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_BothEdge ,TIM_ICPolarity_BothEdge);//ʹ�ñ�����ģʽ3 �������ض�����
//    TIM_ICStructInit(&TIM3_ICInitStructure);//���ṹ���е�����ȱʡ����
//    TIM3_ICInitStructure.TIM_ICFilter = 6;  //ѡ�����벶���˲���
//    TIM_ICInit(TIM3, &TIM3_ICInitStructure);//��TIM_ICInitStructure��ָ��������ʼ��TIM2
//	
//	//�жϷ����ʼ��
//	NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;  //TIM2�ж�
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //��ռ���ȼ�2��
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //�����ȼ�0��
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQͨ����ʹ��
//	NVIC_Init(&NVIC_InitStructure);  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ��� 
//	
//	TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE);//��������ж� ,����CC1IE�����ж�	
//	TIM3->CNT = 0;
//	TIM_Cmd(TIM3,ENABLE ); 	//ʹ�ܶ�ʱ��2
//}	

//��ʱ��2�жϷ������
void TIM2_IRQHandler(void)
{ 	
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
	{	  
			     	    					   
	}	
 
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update); //����жϱ�־λ
 
}
//��ʱ��4�жϷ������
//void TIM3_IRQHandler(void)
//{ 	
//	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
//	{	  
//			     	    					   
//	}	
// 
//    TIM_ClearITPendingBit(TIM3, TIM_IT_Update); //����жϱ�־λ
// 
//}


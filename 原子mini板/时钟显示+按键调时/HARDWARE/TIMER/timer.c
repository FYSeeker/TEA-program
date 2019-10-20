#include "timer.h"
#include "led.h"
#include "display.h"

extern TimeStruct Time;
extern u8 update_flog;

/*********************************************************************
�������ƣ�TIM3_Int_Init			interrupt
�������ܣ�TIM3��ʱ���ж�����
���������arr:�Զ���װ��ֵ
		  psc:Ԥ��Ƶϵ��
���ز�������
*********************************************************************/
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStrue;					//��ʱ����ʼ���ṹ��
	NVIC_InitTypeDef NVIC_InitStrue;								//�жϳ�ʼ���ṹ��
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);			//ʹ�ܶ�ʱ��3ʱ��APB1
	
	TIM_TimeBaseInitStrue.TIM_Period = arr;							//�Զ���װ��ֵ��Period---����
	TIM_TimeBaseInitStrue.TIM_Prescaler = psc;						//Ԥ��Ƶϵ��
	TIM_TimeBaseInitStrue.TIM_CounterMode = TIM_CounterMode_Up;		//����ģʽ�����ϼ���
	TIM_TimeBaseInitStrue.TIM_ClockDivision = TIM_CKD_DIV1;			//ʱ�ӷ�Ƶ��1��Ƶ
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStrue);					//TIM3��ʱ����ʼ��
	
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);						//TIM3��ʱ���жϳ�ʼ��
	
	NVIC_InitStrue.NVIC_IRQChannel = TIM3_IRQn;						//��ʱ��3�ж�
	NVIC_InitStrue.NVIC_IRQChannelCmd = ENABLE;						//ʹ���ж�
	NVIC_InitStrue.NVIC_IRQChannelPreemptionPriority = 0;			//��ռ���ȼ�0
	NVIC_InitStrue.NVIC_IRQChannelSubPriority = 3;					//��Ӧ���ȼ�3
	
	NVIC_Init(&NVIC_InitStrue);										//�жϳ�ʼ��
	
	TIM_Cmd(TIM3, ENABLE);											//TIM3��ʱ��ʹ��
}



/*********************************************************************
�������ƣ�TIM3_IRQHandler
�������ܣ�TIM3��ʱ���жϷ�����
�����������
���ز�������
*********************************************************************/
void TIM3_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM3, TIM_FLAG_Update)!=RESET)		//�ж��ж������Ƿ�ΪTIM3�����ж�
	{	
		Time.sec++;
		update_flog = 1;	//ʱ����±�־
	
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);		//���TIM3�жϱ�־
	}
}

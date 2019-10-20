#include "control.h"
#include "pwm.h"
#include "delay.h"
#include "usart.h"
#include "key.h"



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
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;				//�����ȼ�1��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;					//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);									//����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

	TIM_Cmd(TIM1, ENABLE);
}


/******************************************************************
*��������:	TIM1_UP_IRQHandler
*��������:	��ʱ��1�ĸ����ж�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)			//���ָ����TIM�жϷ������:TIM�ж�Դ
	{
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);				//���TIMx���жϴ�����λ:TIM�ж�Դ					
		
//		printf("cmd!!\r\n");
		if(KEY0==0||KEY1==0||KEY2==0||WK_UP==1)
		{
			if(KEY0==0)
			{
				USART_SendData(USART2,0x04);		//��ת
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));			
			}
			else if(KEY1==0)
			{				
				USART_SendData(USART2,0x02);		//����
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));		
			}
			else if(KEY2==0)
			{				
				USART_SendData(USART2,0x03);		//��ת
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));		
			}
			else if(WK_UP==1)
			{				
				USART_SendData(USART2,0x01);		//ǰ��
				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));		
			}
//			printf("cmd ok\r\n");
		}
	}
}



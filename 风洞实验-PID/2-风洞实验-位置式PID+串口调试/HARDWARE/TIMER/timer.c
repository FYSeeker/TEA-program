#include "timer.h"
#include "led.h"
#include "lcd.h"
#include "pid.h"
#include "ultrasonic.h"

extern float Distance;

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
�������ܣ�TIM2��ʱ������
���������arr:�Զ���װ��ֵ
		  psc:Ԥ��Ƶϵ��
���ز�������
*********************************************************************/


void TIM2_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//��ʱ����ʼ���ṹ��
 	NVIC_InitTypeDef NVIC_InitStructure;				//�жϳ�ʼ���ṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);	//ʹ��TIM2ʱ��
	
	/*��ʼ����ʱ��3*/	 
	TIM_TimeBaseStructure.TIM_Period = arr; 					//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler =psc; 					//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);				//��ʼ��TIM2
	
	/*�жϷ����ʼ��*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;  			//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							//��ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);			//��������ж�	
	
//	TIM_Cmd(TIM2,ENABLE ); 	//ʹ�ܶ�ʱ��2
}


/*********************************************************************
�������ƣ�TIM5_Init
�������ܣ�TIM2��ʱ������
���������arr:�Զ���װ��ֵ
		  psc:Ԥ��Ƶϵ��
���ز�������
*********************************************************************/


void TIM5_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;		//��ʱ����ʼ���ṹ��
 	NVIC_InitTypeDef NVIC_InitStructure;				//�жϳ�ʼ���ṹ��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);	//ʹ��TIM2ʱ��
	
	/*��ʼ����ʱ��5*/	 
	TIM_TimeBaseStructure.TIM_Period = arr; 					//�趨�������Զ���װֵ 
	TIM_TimeBaseStructure.TIM_Prescaler = psc; 					//Ԥ��Ƶ��   
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1; 	//����ʱ�ӷָ�:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up; //TIM���ϼ���ģʽ
	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);				//��ʼ��TIM5
	
	/*�жϷ����ʼ��*/
	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;  			//TIM2�ж�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; 	//��ռ���ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  		//�����ȼ�0��
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 			//IRQͨ����ʹ��
	NVIC_Init(&NVIC_InitStructure);  							//��ʼ������NVIC�Ĵ��� 
	
	TIM_ITConfig(TIM5,TIM_IT_Update,ENABLE);			//��������ж�	
	
	TIM_Cmd(TIM5,ENABLE); 	//ʹ�ܶ�ʱ��5
}



/*********************************************************************
�������ƣ�TIM2_IRQHandler
�������ܣ�TIM2��ʱ���жϷ�����
�����������
���ز�������
*********************************************************************/
void TIM2_IRQHandler(void)
{
	if(TIM_GetFlagStatus(TIM2, TIM_IT_Update)!=RESET)	//�ж��ж�Ϊ�����жϣ���ʱģ��û�н��յ����������������10m
	{
		nvic_flog = 2;		//�������ж�
	}	
	
	TIM_ClearITPendingBit(TIM2,TIM_IT_Update);		//���TIM2�жϱ�־
}


/*********************************************************************
�������ƣ�TIM5_IRQHandler
�������ܣ�TIM5��ʱ���жϷ�����
�����������
���ز�������
*********************************************************************/
void TIM5_IRQHandler(void)
{
	static float pid_value, pid_last_value, pwmvalue;
	
	if(TIM_GetFlagStatus(TIM5, TIM_IT_Update)!=RESET)	//�ж��ж�Ϊ�����ж�,10ms
	{
		/*���ε���ֵ�ۼӣ�Ч�����Ը���*/
		pid_last_value = pid_value;		//��¼�ϴ�PID��ֵ
		pid_value = PID_Adjust(Distance);	//PID��ȡ���ε���ֵ
		pwmvalue = pid_value + pid_last_value;
		TIM_SetCompare1(TIM3,pid_value+5760);		//5730(7200)
		LCD_ShowxNum(92,100,pid_value+5760,5,16,0x80);
		nvic_flog = 1;
	}	
	
	TIM_ClearITPendingBit(TIM5,TIM_IT_Update);		//���TIM2�жϱ�־
}

#include "main.h"
	
u16 AdcValue=0;


/**********************************************************************
�������ܣ���������
��ڲ�������
���ز�������
***********************************************************************/
void Key_Adjust()
{
	u8 KeyVal=0;
	
	KeyVal = KEY_Scan(1);
	if(KeyVal==KEY0_PRES)
	{
		TIM_Cmd(TIM1, DISABLE);
		PWM1 = 2000;
		PWM2 = 0;
		while(KeyVal==KEY0_PRES)		//���ּ��
		{
			delay_ms(10);
			KeyVal = KEY_Scan(1);
		}
		
		PWM1 = 0;
		PWM2 = 0;
		TIM_Cmd(TIM1, ENABLE);
	}
	else if(KeyVal==KEY2_PRES)
	{
		TIM_Cmd(TIM1, DISABLE);
		PWM1 = 0;
		PWM2 = 2000;
		while(KeyVal==KEY2_PRES)		//���ּ��
		{
			delay_ms(10);
			KeyVal = KEY_Scan(1);
		}
		PWM1 = 0;
		PWM2 = 0;
		TIM_Cmd(TIM1, ENABLE);
	}
}


/**********************************************************************
�������ܣ�������
��ڲ�������
���ز�������
***********************************************************************/
int main(void)
{
	
	delay_init();				//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(UAR1,115200);	 		//���ڳ�ʼ��Ϊ115200
	
	LED_Init();					//LED�˿ڳ�ʼ��
	KEY_Init();					//������ʼ�� 	
	LCD_Init();					//LCD��ʼ��
 	tp_dev.init();				//��������ʼ��
	Adc_Init();					//ADC��ʼ����PA1
	MYDMA_Config(DMA1_Channel1,(u32)&ADC1->DR,(u32)&AdcValue,64);	//��ʼ��DMA
	ADC_DMACmd(ADC1,ENABLE);  	//ʹ��DMA����ADC��ֵ
	
	Interface_Init();			//��ʼ����
	
	Tim4EncoderInit();			//��������ʼ��
	TIM3_PWM_init(7199,0);		//��ʱ��PWM����ʼ����10KHz
	TIM1_IntInit(4999,71);		//��ʼ����ʱ��1,����10ms�ж�

	while(1)
	{
		Angle = Count_Angle(AdcValue);			//����Ƕ�	
		TouchKey_Set();							//��������
		Operation_Interface(AdcValue);			//��������
		Uart_DealDate();						//�������ݴ���
		Key_Adjust();							//��������
		
		/*��������*/
		if(Run)
		{			
			
			switch(Key_Flag)
			{
				case 1:TIM_Cmd(TIM1, ENABLE);
					break;
				case 2:Procedure_2();
					break;
				case 3:
				case 4:
				case 5:
				case 6:TIM_Cmd(TIM1, ENABLE);
					break;
			}
		}
		else
		{
			TIM_Cmd(TIM1, DISABLE);
			IntTime = 0;
			PWM1 = 0;
			PWM2 = 0;
		}
	}
}


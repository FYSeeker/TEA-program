#include "main.h"


/**********************************************************************
�������ܣ�������
��ڲ�������
���ز�������
***********************************************************************/
int main(void)
{	
	u8 rx_sta=0;
	
	delay_init();				//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(UAR1,115200);	 		//���ڳ�ʼ��Ϊ115200	
	uart_init(UAR2,1382400);	 	//���ڳ�ʼ��Ϊ1382400	
	LED_Init();					//LED�˿ڳ�ʼ��
	KEY_Init();					//������ʼ�� 	
//	BluetoothIO_init();			//�����˿ڳ�ʼ��
	LCD_Init();					//LCD��ʼ��
	mallco_dev.init(SRAMIN);	//��ʼ���ڲ��ڴ��
	RX_buf = mymalloc(SRAMIN,RxSize);
	if(RX_buf==NULL)
	{
		while(1)
		{
			LED1 = !LED1;
			delay_ms(200);
		}
	}
	else
	{
		printf("RX_buf OK!\r\n");
	}
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		delay_ms(100);
		LED1 = !LED1;
	}
	LED1 = 1;
	NRF24L01_RX_Mode();			//��ʼ�����ߵ�����ģʽ	
	
	MYDMA_Config(DMA1_Channel6,(u32)&USART2->DR,(u32)RX_buf,RxSize);
	MYDMA_Enable();	
//	MYDMA_SRAMLCD_Init((u32)RX_buf);//RAMд��LCD
	
	TIM1_IntInit(99,127);		//��ʱ���ж�
	
//	while(Bluetooth_P)			//�ȴ���������
//	{
//		delay_ms(1);
//	}
//	delay_ms(1200);
	
	USART_SendData(USART2,0xf0);		//����0xf0
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));	
	
	while(1)
	{	
		if(!NRF24L01_RxPacket(&rx_sta))		//�ж�֡��ʼ��־
		{		
			if(rx_sta==1)
			{
				LCD_SetCursor(0, 0);		//��������
				LCD_WriteRAM_Prepare();		//��ʼд��GRAM
				rx_sta = 0;
			}
		}
	}
}


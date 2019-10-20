#include "main.h"



/*���߽���BUF*/
u8 Rxbuf[32]={0};

/**********************************************************************
�������ܣ�������
��ڲ�������
���ز�������
***********************************************************************/
int main(void)
{
	u8 i,key=0;
	u16 x=0;
	
	delay_init();				//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(UAR1,115200);	 	//���ڳ�ʼ��Ϊ115200
	MyUSART2_init(1382400);	
	LED_Init();					//LED�˿ڳ�ʼ��
	KEY_Init();					//������ʼ��
	LCD_Init();					//LCD��ʼ��
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		delay_ms(100);
		LED1 = !LED1;
	}
	LED1 = 1;
	NRF24L01_RX_Mode();			//��ʼ�����ߵ�����ģʽ	

//	MYDMA_Config(DMA1_Channel4,(u32)&SPI2->DR,(u32)Rxbuf,32);
//	EXTI6_Init();
	
	USART_DMAConfig(DMA1_Channel6,(u32)&USART2->DR,(u32)&Rx_sta,1);
	
	TIM1_IntInit(199,71);		//��ʼ����ʱ���ж�,20ms
	
	USART_SendData(USART2,0xf0);		//����0xf0
	while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	
	while(1)
	{				
		if(!NRF24L01_RxPacket(Rxbuf))
		{		
			USART_SendData(USART2,0xf0);		//����0xf0
			while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
			
			for(i=0;i<32;i+=2)
			{
				LCD->LCD_RAM=(Rxbuf[i]<<8)+Rxbuf[i+1];	
			}
			
//			x++;
//			if(x==4800)
//			{
//				USART_SendData(USART2,0xf0);		//����0xf0
//				while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
//				x = 0;
//			}
		}
		
//		if(Rx_sta==0xff)
//		{			
//			USART_SendData(USART2,0xf0);		//����0xf0
//			while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
//			Rx_sta=0;
//			printf("ff\r\n");
//			LCD_SetCursor(0,0);	//���ù��λ�� 
//			LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
//		}

//		printf("%x\t",Rx_sta);
//		Rx_sta = 0;
//		delay_ms(500);
	}
}


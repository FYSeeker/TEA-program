#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "lcd.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "EasyTracer.h"
#include "key.h"
#include "24l01.h"
#include "dma.h"
#include "control.h"


int main(void)
{	
	u8 rx_sta=0;
	
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART1
    MyUSART2_Init(1382400);         //��ʼ��USART2
	LED_Init();                     //��ʼ��LED 
	KEY_Init();						//��ʼ��KEY
	LCD_Init();	                    //��ʼ��LCD
	
	NRF24L01_Init();	//���߳�ʼ��
	while(NRF24L01_Check())
	{
		LED1=!LED1;
		delay_ms(200);
	}
	NRF24L01_RX_Mode();	//��������Ϊ����ģʽ 
	
//	DMA1Stream5_4_init((u32)USART2->DR,(u32)RX_buf,1);
	TIM3_Init(19999,89);		//��ʱ����ʼ��
	
	printf("init ok!\r\n");
	
	USART2->DR = 0xf0;
	while(!(USART2->SR & (1<<6))){}	//�ȴ��������
	
    while(1)
    {	
//		if(!NRF24L01_RxPacket(&rx_sta))		//�ж�֡��ʼ��־
//		{		
//			if(rx_sta==1)
//			{
//				LCD_SetCursor(0, 0);		//��������
//				LCD_WriteRAM_Prepare();		//��ʼд��GRAM
//				rx_sta = 0;
//			}
//		}
//		printf("%d\r\n",RX_buf[0]);
//		delay_ms(500);	
	}
}

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "ltdc.h"
#include "string.h"
#include "sdram.h"
#include "malloc.h"
#include "w25qxx.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "pcf8574.h" 
#include "ff.h"
#include "exfuns.h"
#include "string.h"
#include "usmart.h"
#include "sdio_sdcard.h"
#include "fontupd.h"
#include "text.h"
#include "piclib.h"	
#include "string.h"		
#include "math.h"
#include "timer.h"
#include "EasyTracer.h"


/*********************************************************************
�������ܣ�	������
��ڲ�����	��
���ز�����	��
**********************************************************************/
int main(void)
{	
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
	HAL_Init();                     //��ʼ��HAL��
	delay_init(180);                //��ʼ����ʱ����
	uart_init(921600);              //��ʼ��USART
//	usmart_dev.init(90);            //��ʼ��USMART
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	LCD_Init();                     //LCD��ʼ��
	
	LCD_ShowString(20,20,200,24,24,(u8*)"OV2640 test");		
	
//	my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
	  	
//	dcmi_line_buf[0]=mymalloc(SRAMIN,jpeg_line_size*4);	//Ϊjpeg dma���������ڴ�	
//	dcmi_line_buf[1]=mymalloc(SRAMIN,jpeg_line_size*4);	//Ϊjpeg dma���������ڴ�

	while(OV2640_Init())//��ʼ��OV2640
	{
		LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 Error!!!");
		delay_ms(200);
		LCD_Fill(20,50,220,80,BLACK);
		delay_ms(200);
	}
	LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 OK!"); 
	delay_ms(500);
	OV2640_Config();	//OV2640��ʼ����
	
	DCMI_Init();		//DCMI����	
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);	//DCMI DMA����,MCU��
	
	LCD_Clear(BLACK);	
	TIM3_Init(10000-1,9000-1);		//10Khz����,1�����ж�һ��
	DCMI_Start(); 					//�������� 
	
	while(1)
	{		
		if(frame_over)	//һ֡�������
		{
			Find_Midline();		//Ѱ�Һ���

			frame_over = 0;
			DCMI_Start();
		}
		
	}
}

#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "key.h"
#include "lcd.h"
#include "malloc.h"
#include "ov2640.h" 
#include "dcmi.h"  
#include "usmart.h"
#include "timer.h"
#include "motor.h"
#include "EasyTracer.h"


/*********************************************************************
�������ܣ�	������
��ڲ�����	��
���ز�����	��
**********************************************************************/
int main(void)
{	
	u8 key=0;
	
	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
	HAL_Init();                     //��ʼ��HAL��
	delay_init(180);                //��ʼ����ʱ����
	uart_init(921600);              //��ʼ��USART
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	LCD_Init();                     //LCD��ʼ��	
	
	LCD_ShowString(20,20,200,24,24,(u8*)"OV2640 test");		

	while(OV2640_Init())//��ʼ��OV2640
	{
		LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 Error!!!");
		delay_ms(200);
		LCD_Fill(20,50,220,80,BLACK);
		delay_ms(200);
	}
	LCD_ShowString(20,50,200,24,24,(u8*)"OV2640 OK!"); 
	delay_ms(500);
	OV2640_Config();		//OV2640��ʼ����
	
	DCMI_Init();			//DCMI����	
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);	//DCMI_DMA����,MCU��	
	LCD_Clear(BLACK);		//����
	DCMI_Start(); 			//�������� 
	
	TIM2_PWM_Init(8999,0);	//TIM��PWM��ʼ�� ,10KHz
	TIM1_Init(9999,1799);	//��ʱ���жϣ�100ms
	
	while(1)
	{				
		if(frame_over)	//һ֡�������
		{
			Find_Midline();		//Ѱ�Һ���
			
			/*������*/
			POINT_COLOR = RED;
			LCD_DrawLine(160,100,160,140);
			POINT_COLOR = WHITE;
			
			frame_over = 0;
			DCMI_Start();
		}
		
		key = KEY_Scan(0);
		if(key == KEY0_PRES)
		{
			Run = !Run;
		}
	}
}

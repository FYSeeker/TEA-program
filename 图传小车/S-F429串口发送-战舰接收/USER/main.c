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


extern u8 U2Rx_sta;	//����2���ձ��

//���ݷ���BUF
extern u8 Txbuf[RxSize];	//usart.c

//	y x
u16 img_buf[240][320];

//// .
//RESULT Resu;           
//TARGET_CONDI Condition={120,180,50,240,20,190,50,50,100,100};


int main(void)
{
	u16 x, y, bufcnt=0;		
	
    HAL_Init();                     //��ʼ��HAL��   
    Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz
    delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART1
    MyUSART2_Init(1382400);         //��ʼ��USART2 2764800
	LED_Init();                     //��ʼ��LED 
	KEY_Init();						//��ʼ��KEY
	LCD_Init();	                    //��ʼ��LCD 
	
	while(OV2640_Init())//��ʼ��OV2640
	{
		LCD_ShowString(60,50,lcddev.width,16,16,(u8*)"OV2640 Error!");
		delay_ms(300);				  
		LCD_Fill(60,50,lcddev.width,70,BLACK);//�����ʾ
		delay_ms(200);
		LED1=!LED1;
	}
	LED1 = 1;
	
	OV2640_RGB565_Mode();	//RGB565ģʽ 
	OV2640_Light_Mode(0);	//�Զ�ģʽ
	OV2640_Color_Saturation(2);//ɫ�ʱ��Ͷ�2
	OV2640_Brightness(2);	//����0
	OV2640_Contrast(2);		//�Աȶ�0
	
	OV2640_OutSize_Set(320,240);
	
	DCMI_Init();			//DCMI����	
	DCMI_DMA_Init((u32)img_buf,0,sizeof(img_buf)/4,DMA_MDATAALIGN_HALFWORD,DMA_MINC_ENABLE);//DCMI DMA����					
	DCMI_Start(); 			//�������� 
		
    while(1)
    {	
		LCD_SetCursor(0, 0);
		LCD_WriteRAM_Prepare();		//��ʼд��GRAM
		
		for (y = 0; y < 240; y++)
		{
			for (x = 0; x < 320; x++)
			{
				// ��ʾԭͼ
				LCD->LCD_RAM = img_buf[y][x];
									
				Txbuf[bufcnt] = (img_buf[y][x]>>8);
				Txbuf[bufcnt+1] = (img_buf[y][x]);
				bufcnt+=2;
				
				if(x==319)
				{	
					LCD_SetCursor(0, y+1);
					LCD_WriteRAM_Prepare();		//��ʼд��GRAM
				}
				
				if(bufcnt==RxSize)					//��������BUF
				{
					while(!U2Rx_sta)				//�ȴ�Ӧ��U2Rx_sta�ж��д���	
					{
						My_NOP();					//�պ���
					}
					MyUSART_SendData(Txbuf,RxSize);	//��������
					U2Rx_sta = 0;
					bufcnt = 0;
				}
			}
		}
		
		


		
		// ����С��
//		if(Trace(&Condition,&Resu))
//		{
//			LCD_DrawRectangle(Resu.x-Resu.w/2+2,Resu.y-Resu.h/2+2,Resu.x+Resu.w/2-2,Resu.y+Resu.h/2-2);
//			LCD_DrawRectangle(Resu.x-2,Resu.y-2,Resu.x+2,Resu.y+2);
//		}
//		
//		// ��ʾ��������
//		LCD_ShowxNum(280, 190, Resu.x, 3, 16, 0);
//		LCD_ShowxNum(280, 210, Resu.y, 3, 16, 0);
//		
//		// ��������
//		uart1_send_data(Resu.x, Resu.y, Resu.w, Resu.h);
	}
}

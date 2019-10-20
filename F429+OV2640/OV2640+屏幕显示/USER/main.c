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
/************************************************
 ALIENTEK ������STM32F429������ʵ��44
 �����ʵ��-HAL�⺯����
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com  
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

vu8 bmp_request=0;						//bmp��������:0,��bmp��������;1,��bmp��������,��Ҫ��֡�ж�����,�ر�DCMI�ӿ�.
u8 ovx_mode=0;							//bit0:0,RGB565ģʽ;1,JPEGģʽ 
u16 curline=0;							//����ͷ�������,��ǰ�б��
u16 yoffset=0;							//y�����ƫ����

#define jpeg_buf_size   4*1024*1024		//����JPEG���ݻ���jpeg_buf�Ĵ�С(4M�ֽ�)
#define jpeg_line_size	2*1024			//����DMA��������ʱ,һ�����ݵ����ֵ

u32 *dcmi_line_buf[2];					//RGB��ʱ,����ͷ����һ��һ�ж�ȡ,�����л���  
u32 *jpeg_data_buf;						//JPEG���ݻ���buf 

volatile u32 jpeg_data_len=0; 			//buf�е�JPEG��Ч���ݳ��� 
volatile u8 jpeg_data_ok=0;				//JPEG���ݲɼ���ɱ�־ 
										//0,����û�вɼ���;
										//1,���ݲɼ�����,���ǻ�û����;
										//2,�����Ѿ����������,���Կ�ʼ��һ֡����
//����JPEG����
//���ɼ���һ֡JPEG���ݺ�,���ô˺���,�л�JPEG BUF.��ʼ��һ֡�ɼ�.
void jpeg_data_process(void)
{
	u16 i;
	u16 rlen;			//ʣ�����ݳ���
	u32 *pbuf;
	curline=yoffset;	//������λ
	if(ovx_mode&0X01)	//ֻ����JPEG��ʽ��,����Ҫ������.
	{
		if(jpeg_data_ok==0)	//jpeg���ݻ�δ�ɼ���?
		{
			__HAL_DMA_DISABLE(&DMADMCI_Handler);//�ر�DMA
			while(DMA2_Stream1->CR&0X01);	//�ȴ�DMA2_Stream1������ 
			rlen=jpeg_line_size-__HAL_DMA_GET_COUNTER(&DMADMCI_Handler);//�õ�ʣ�����ݳ���	
			pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ,�������
			if(DMADMCI_Handler.Instance->CR&(1<<19))for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[1][i];//��ȡbuf1�����ʣ������
			else for(i=0;i<rlen;i++)pbuf[i]=dcmi_line_buf[0][i];//��ȡbuf0�����ʣ������ 
			jpeg_data_len+=rlen;			//����ʣ�೤��
			jpeg_data_ok=1; 				//���JPEG���ݲɼ��갴��,�ȴ�������������
		}
		if(jpeg_data_ok==2)	//��һ�ε�jpeg�����Ѿ���������
		{
            __HAL_DMA_SET_COUNTER(&DMADMCI_Handler,jpeg_line_size);	//���䳤��Ϊjpeg_buf_size*4�ֽ�
			__HAL_DMA_ENABLE(&DMADMCI_Handler); //��DMA
			jpeg_data_ok=0;					//�������δ�ɼ�
			jpeg_data_len=0;				//�������¿�ʼ
		}
	}else
	{  
		if(bmp_request==1)	//��bmp��������,�ر�DCMI
		{
			DCMI_Stop();	//ֹͣDCMI
			bmp_request=0;	//������������.
		}
		LCD_SetCursor(0,0);  
		LCD_WriteRAM_Prepare();				//��ʼд��GRAM  
	}  
} 

//jpeg���ݽ��ջص�����
void jpeg_dcmi_rx_callback(void)
{  
	u16 i;
	u32 *pbuf;
	pbuf=jpeg_data_buf+jpeg_data_len;//ƫ�Ƶ���Ч����ĩβ
	if(DMADMCI_Handler.Instance->CR&(1<<19))//buf0����,��������buf1
	{ 
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[0][i];//��ȡbuf0���������
		jpeg_data_len+=jpeg_line_size;//ƫ��
	}else //buf1����,��������buf0
	{
		for(i=0;i<jpeg_line_size;i++)pbuf[i]=dcmi_line_buf[1][i];//��ȡbuf1���������
		jpeg_data_len+=jpeg_line_size;//ƫ�� 
	} 
} 

//RGB�����ݽ��ջص�����
void rgblcd_dcmi_rx_callback(void)
{  
	u16 *pbuf;
	if(DMADMCI_Handler.Instance->CR&(1<<19))//DMAʹ��buf1,��ȡbuf0
	{ 
		pbuf=(u16*)dcmi_line_buf[0]; 
	}else 						//DMAʹ��buf0,��ȡbuf1
	{
		pbuf=(u16*)dcmi_line_buf[1]; 
	} 	
	LTDC_Color_Fill(0,curline,lcddev.width-1,curline,pbuf);//DM2D��� 
	if(curline<lcddev.height)curline++;
	if(bmp_request==1&&curline==(lcddev.height-1))//��bmp��������,�ر�DCMI
	{
		DCMI_Stop();	//ֹͣDCMI
		bmp_request=0;	//������������.
	}
}



int main(void)
{	
//	u8 key;					        //��ֵ	
//	u8 scale=1;				        //Ĭ����ȫ�ߴ�����
	
	u16 i;					

	Stm32_Clock_Init(360,25,2,8);   //����ʱ��,180Mhz   
	HAL_Init();                     //��ʼ��HAL��
	delay_init(180);                //��ʼ����ʱ����
	uart_init(115200);              //��ʼ��USART
	usmart_dev.init(90);            //��ʼ��USMART
	LED_Init();                     //��ʼ��LED 
	KEY_Init();                     //��ʼ������
	LCD_Init();                     //LCD��ʼ��
	
	LCD_ShowString(30,70,200,24,24,(u8*)"OV2640 test");		
	
	my_mem_init(SRAMIN);            //��ʼ���ڲ��ڴ��
//	my_mem_init(SRAMEX);            //��ʼ���ⲿSDRAM�ڴ��
//	my_mem_init(SRAMCCM);           //��ʼ���ڲ�CCM�ڴ��
	  	
	dcmi_line_buf[0]=mymalloc(SRAMIN,jpeg_line_size*4);	//Ϊjpeg dma���������ڴ�	
	dcmi_line_buf[1]=mymalloc(SRAMIN,jpeg_line_size*4);	//Ϊjpeg dma���������ڴ�

	while(OV2640_Init())//��ʼ��OV2640
	{
		LCD_ShowString(30,100,200,24,24,(u8*)"OV2640 Error!!!");
		delay_ms(200);
		LCD_Fill(30,100,230,130,BLACK);
		delay_ms(200);
	}
	LCD_ShowString(30,100,200,24,24,(u8*)"OV2640 OK!"); 
	delay_ms(500);
	
	//�Զ��Խ���ʼ��
	OV2640_RGB565_Mode();	//RGB565ģʽ 
	OV2640_Light_Mode(0);	//�Զ�ģʽ
	OV2640_Color_Saturation(2);//ɫ�ʱ��Ͷ�0
	OV2640_Brightness(2);	//����0
	OV2640_Contrast(2);		//�Աȶ�0
	
//	OV2640_ImageWin_Set(0,0,1600,1200);			 
	OV2640_OutSize_Set(640,480);	//4:3
	
	DCMI_Init();			//DCMI����
	
	DCMI_DMA_Init((u32)&LCD->LCD_RAM,0,1,DMA_MDATAALIGN_HALFWORD,DMA_MINC_DISABLE);	//DCMI DMA����,MCU��
	
	LCD_Clear(BLACK);
	DCMI_Start(); 			//�������� 
	
	while(1)
	{	
//		key=KEY_Scan(0);//��֧������
		
		delay_ms(1);
		i++;
		if(i>499)//DS0��˸.
		{
			i=0;
			LED0=!LED0;
		}
	}
}

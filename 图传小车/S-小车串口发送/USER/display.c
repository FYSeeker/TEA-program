#include "display.h"
#include "ov7670.h"
#include "24l01.h"
#include "exti.h"
#include "timer.h"
#include "control.h"
#include "usart.h"
#include "delay.h"
#include "dma.h"

u8 TX_sta=1;

/*����buf*/
u8* TX_buf;

/*ͼ�����ݴ���*/
void Image_DataHandle(void)
{
	u32 n;
	u16 PixCnt=0;
	
	if(ov_sta==2)
	{		
		NRF24L01_TxPacket(&TX_sta);	//��ʼ��һ֡
		
 		OV7670_RRST=0;			//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H; 
		OV7670_RCK_L;
		OV7670_RRST=1;			//��λ��ָ����� 
		OV7670_RCK_H; 
		
		for(n=0;n<(OvLcdWide*OvLcdHigh);n++)
		{	    
			OV7670_RCK_L;
			TX_buf[PixCnt]=OV7670_DATA;		//������,�߰�λ
			OV7670_RCK_H; 
			 
			OV7670_RCK_L;
			TX_buf[PixCnt+1]=OV7670_DATA;	//������,�Ͱ�λ	
			OV7670_RCK_H; 
			
			PixCnt+=2;
			if(PixCnt==buf_size)
			{
				while((USART3_flag&0xf0)!=0xf0);//�ȴ�Ӧ��
				DMA1_Channel2_Enable();			//����DMA����
				USART3_flag&=0x0f;				//���Ӧ��
				PixCnt = 0;
			}
		}
					  
		OV7670_RCK_L;
		OV7670_RCK_H; 
		ov_sta=0;				//��ʼ��һ�βɼ�
// 		ov_frame++;  				 	 
	} 
}



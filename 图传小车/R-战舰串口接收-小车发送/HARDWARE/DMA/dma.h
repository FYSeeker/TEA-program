#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"

/*����ͷ������С*/
#define OvLcdWide 240
#define OvLcdHigh 320

/*���ݽ���BUF��С*/
#define RxSize OvLcdHigh*2*10

/*���ձ��*/
extern u8 RX_sta;

/*���ݽ���BUF*/
extern u8* RX_buf;


void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//����DMA1_CHx

//����DMA����
void MYDMA_Enable(void);//ʹ��DMA

//�ر�DMA����
void MYDMA_Disable(void);

/*DMAͨ��7*/
void DMAChannel7_Config(u32 cpar,u32 cmar,u16 cndtr);
//����DMA����
void DMAChannel7_Enable(void);
//�ر�DMA����
void DMAChannel7_Disable(void);


//SRAM --> LCD_RAM dma����
//caddr������Դ��ַ
//16λ,��SRAM���䵽LCD_RAM. 
void MYDMA_SRAMLCD_Init(u32 caddr);
//����һ��SRAM��LCD��DMA�Ĵ���
void MYDMA_SRAMLCD_Enable(void);
		   
#endif





#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"


/*���ݽ���BUF��С*/
#define RxSize 640*2							    					    

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//����DMA1_CHx

//����DMA����
void MYDMA_Enable(void);//ʹ��DMA

//�ر�DMA����
void MYDMA_Disable(void);
		   
#endif





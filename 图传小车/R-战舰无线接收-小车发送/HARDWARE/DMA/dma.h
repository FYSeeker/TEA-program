#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"
							    					    

void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);//����DMA1_CHx

//����DMA����
void MYDMA_Enable(void);//ʹ��DMA

//�ر�DMA����
void MYDMA_Disable(void);


void USART_DMAConfig(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr);

//����DMA����
void USART_DMAEnable(void);

//�ر�DMA����
void USART_DMADisable(void);
		   
#endif





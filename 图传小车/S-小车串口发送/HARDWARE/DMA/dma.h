#ifndef __DMA_H
#define	__DMA_H	   
#include "sys.h"						    					    

/*DMA1ͨ��3����*/
void DMA1_Channel3_Config(u32 cpar,u32 cmar,u16 cndtr);//����DMA1_CHx
//����DMA����
void MYDMA_Enable(void);//ʹ��DMA
//�ر�DMA����
void MYDMA_Disable(void);


/*DMA1ͨ��2����*/
void DMA1_Channel2_Config(u32 cpar,u32 cmar,u16 cndtr);
//����DMA����
void DMA1_Channel2_Enable(void);
//�ر�DMA����
void DMA1_Channel2_Disable(void);
		   
#endif





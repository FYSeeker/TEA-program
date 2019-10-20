#include "dma.h"
#include "lcd.h"
#include "usart.h"
#include "display.h"


DMA_InitTypeDef DMA_InitStructure;
NVIC_InitTypeDef NVIC_Initstur;

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 	   

//DMA1ͨ������
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void DMA1_Channel3_Config(u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA1_Channel3);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;					//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			//������ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel3, &DMA_InitStructure);		//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
//	NVIC_Initstur.NVIC_IRQChannel = DMA1_Channel3_IRQn;
//	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_Initstur);
	
//	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);	//����DMA��������ж�
	
	DMA_Cmd(DMA1_Channel3,ENABLE);	//ʹ��DMA����
}


//����DMA����
void MYDMA_Enable()
{ 	
	DMA_Cmd(DMA1_Channel3,ENABLE);
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);
}

//�ر�DMA����
void MYDMA_Disable()
{ 	
	USART_DMACmd(USART3,USART_DMAReq_Rx,DISABLE);
	DMA_Cmd(DMA1_Channel3,DISABLE);
}	  



/*DMA1ͨ��3�жϷ�����*/
void DMA1_Channel3_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA1_IT_TC3) != RESET)	//DMAͨ����������ж�
	{
		DMA_ClearITPendingBit(DMA1_IT_TC3);		//����жϱ�־
		
	}
	
}



/*********************************************************************************/

//DMA1�ĸ�ͨ������
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void DMA1_Channel2_Config(u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA1_Channel2);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;  //���ݴ��䷽�򣬴��ڴ��ȡ���͵�����
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;			//�����ڵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel2, &DMA_InitStructure);		//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
}


//����DMA����
void DMA1_Channel2_Enable()
{ 	
	DMA_Cmd(DMA1_Channel2, DISABLE);  	//�ر�DMA1��ָʾ��ͨ��      
 	DMA_SetCurrDataCounter(DMA1_Channel2,buf_size);//DMAͨ����DMA����Ĵ�С
	DMA_Cmd(DMA1_Channel2,ENABLE);
}

//�ر�DMA����
void DMA1_Channel2_Disable()
{ 	
	USART_DMACmd(USART3,USART_DMAReq_Tx,DISABLE);
	DMA_Cmd(DMA1_Channel2,DISABLE);
}	  






















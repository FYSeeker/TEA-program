#include "dma.h"
#include "lcd.h"
#include "usart.h"
#include "delay.h"


DMA_InitTypeDef DMA_InitStructure;
NVIC_InitTypeDef NVIC_Initstur;

u16 DMA1_MEM_LEN;//����DMAÿ�����ݴ��͵ĳ��� 

//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//�Ӵ洢��->����ģʽ/8λ���ݿ��/�洢������ģʽ
//DMA_CHx:DMAͨ��CHx
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void MYDMA_Config(DMA_Channel_TypeDef* DMA_CHx,u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA_CHx);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA1_MEM_LEN=cndtr;
	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������ȡ���͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;			//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;						//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;		//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;				//���ݿ��Ϊ8λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;			//������ѭ��ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_High;		//DMAͨ�� xӵ�и����ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;			//DMAͨ��xû������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA_CHx, &DMA_InitStructure);		//����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��USART1_Tx_DMA_Channel����ʶ�ļĴ���
	
	NVIC_Initstur.NVIC_IRQChannel = DMA1_Channel6_IRQn;
	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Initstur.NVIC_IRQChannelSubPriority = 1;
	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstur);
	
	DMA_ITConfig(DMA_CHx,DMA_IT_TC,ENABLE);	//����DMA��������ж�
	
	DMA_Cmd(DMA_CHx,DISABLE);	//ʧ��DMA����
}


//����DMA����
void MYDMA_Enable()
{ 	
	DMA_Cmd(DMA1_Channel6,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Rx,ENABLE);
}

//�ر�DMA����
void MYDMA_Disable()
{ 	
	USART_DMACmd(USART2,USART_DMAReq_Rx,DISABLE);
	DMA_Cmd(DMA1_Channel6,DISABLE);
}	  


/*���ձ��*/
u8 RX_sta=0;

/*���ݽ���BUF*/
u8* RX_buf;

/*DMA1ͨ��6�жϷ�����*/
void DMA1_Channel6_IRQHandler(void)
{
	static u16 i,PixCount=0,Line=1;
	
	if(DMA_GetITStatus(DMA1_IT_TC6) != RESET)	//DMAͨ����������ж�
	{
		DMA_ClearITPendingBit(DMA1_IT_TC6);		//����жϱ�־
	
		USART_SendData(USART2,0xf0);		//����0xf0
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
		
		for(i=0;i<RxSize;i+=2)
		{
			LCD->LCD_RAM = (RX_buf[i]<<8)+RX_buf[i+1];
//			PixCount++;
//			if(PixCount==OvLcdHigh)
//			{
//				PixCount=0;
//				LCD_SetCursor(0, Line);		//��������
//				LCD_WriteRAM_Prepare();		//��ʼд��GRAM
//				Line++;
//				if(Line==OvLcdWide-1) Line=0;			
//			}
		}	
		
//		MYDMA_SRAMLCD_Enable();
	}
	
}



/***********************************************************************/


//DMA1�ĸ�ͨ������
//����Ĵ�����ʽ�ǹ̶���,���Ҫ���ݲ�ͬ��������޸�
//cpar:�����ַ
//cmar:�洢����ַ
//cndtr:���ݴ����� 
void DMAChannel7_Config(u32 cpar,u32 cmar,u16 cndtr)
{
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA1_Channel7);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = cpar;	//DMA�������ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = cmar;  		//DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴����跢�͵��ڴ�
	DMA_InitStructure.DMA_BufferSize = cndtr;			//DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;			//�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;						//�ڴ��ַ�Ĵ�������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;		//���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;			//���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;			//�����ڵ���ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium;	//DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;			//DMAͨ������Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA1_Channel7, &DMA_InitStructure);			//��ʼ��DMA��ͨ��
	
//	NVIC_Initstur.NVIC_IRQChannel = DMA1_Channel6_IRQn;
//	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelSubPriority = 1;
//	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_Initstur);
	
//	DMA_ITConfig(DMA1_Channel7,DMA_IT_TC,ENABLE);	//����DMA��������ж�
	
	DMA_Cmd(DMA1_Channel7,DISABLE);	//ʧ��DMA����
}


//����DMA����
void DMAChannel7_Enable()
{ 	
	DMA_Cmd(DMA1_Channel7,ENABLE);
	USART_DMACmd(USART2,USART_DMAReq_Tx,ENABLE);
}

//�ر�DMA����
void DMAChannel7_Disable()
{ 	
	USART_DMACmd(USART2,USART_DMAReq_Tx,DISABLE);
	DMA_Cmd(DMA1_Channel7,DISABLE);
}	  





/***********************************************************************/

//SRAM --> LCD_RAM dma����
//caddr������Դ��ַ
//16λ,��SRAM���䵽LCD_RAM. 
void MYDMA_SRAMLCD_Init(u32 caddr)
{  
	DMA_InitTypeDef DMA_InitStructure;
	
 	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA2, ENABLE);	//ʹ��DMA����
	
	DMA_DeInit(DMA2_Channel5);   //��DMA��ͨ��1�Ĵ�������Ϊȱʡֵ

	DMA_InitStructure.DMA_PeripheralBaseAddr = caddr;  //DMA����ADC����ַ
	DMA_InitStructure.DMA_MemoryBaseAddr = (u32)&LCD->LCD_RAM;  //DMA�ڴ����ַ
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;  //���ݴ��䷽�򣬴������
	DMA_InitStructure.DMA_BufferSize = 0;  //DMAͨ����DMA����Ĵ�С
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Enable;  //�����ַ�Ĵ�������
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;  //�ڴ��ַ�Ĵ���������
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;  //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord; //���ݿ��Ϊ16λ
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;  //��������������ģʽ
	DMA_InitStructure.DMA_Priority = DMA_Priority_Medium; //DMAͨ�� xӵ�������ȼ� 
	DMA_InitStructure.DMA_M2M = DMA_M2M_Enable;  //DMAͨ��x����Ϊ�ڴ浽�ڴ洫��
	DMA_Init(DMA2_Channel5, &DMA_InitStructure);  //����DMA_InitStruct��ָ���Ĳ�����ʼ��DMA��ͨ��
	
	NVIC_Initstur.NVIC_IRQChannel = DMA2_Channel4_5_IRQn;
	NVIC_Initstur.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_Initstur.NVIC_IRQChannelSubPriority = 2;
	NVIC_Initstur.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_Initstur);
	
	DMA_ITConfig(DMA2_Channel5,DMA_IT_TC,ENABLE);	//����DMA��������ж�
}  

//����һ��SRAM��LCD��DMA�Ĵ���
void MYDMA_SRAMLCD_Enable(void)
{
	DMA_Cmd(DMA2_Channel5, DISABLE ); //�ر�DMA����        
 	DMA_SetCurrDataCounter(DMA2_Channel5,RxSize/2);//DMAͨ����DMA����Ĵ�С
 	DMA_Cmd(DMA2_Channel5, ENABLE);  //ʹ��USART1 TX DMA1 ��ָʾ��ͨ�� 	
} 




/*DMA�жϷ�����*/
void DMA2_Channel4_5_IRQHandler(void)
{
	if(DMA_GetITStatus(DMA2_IT_TC5) != RESET)	//DMAͨ����������ж�
	{
		DMA_ClearITPendingBit(DMA2_IT_TC5);		//����жϱ�־
		
		USART_SendData(USART2,0x66);		//����0x66
		while(!USART_GetFlagStatus(USART2,USART_FLAG_TXE));
	}
	
}










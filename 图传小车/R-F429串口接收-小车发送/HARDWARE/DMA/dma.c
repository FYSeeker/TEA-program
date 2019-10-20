#include "dma.h"
#include "lcd.h"
#include "usart.h"


/*******************************************************************************/


DMA_HandleTypeDef UART2RxDMA_Handler;


//DMA����
//mem0addr:�洢����ַ0  ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//mem1addr:�洢����ַ1  ��ֻʹ��mem0addr��ʱ��,��ֵ����Ϊ0
//memsize:����Դ��Ŀ�ĵش�������ݳ���
void DMA1Stream5_4_init(u32 mem0addr,u32 mem1addr,u16 memsize)
{ 
    __HAL_RCC_DMA1_CLK_ENABLE();                                    //ʹ��DMA1ʱ��
    __HAL_LINKDMA(&UART2_Handle,hdmarx,UART2RxDMA_Handler);     //��DMA��USART2��ϵ����
	
    UART2RxDMA_Handler.Instance=DMA1_Stream5;                          //DMA1������5                    
    UART2RxDMA_Handler.Init.Channel=DMA_CHANNEL_4;                     //ͨ��4
    UART2RxDMA_Handler.Init.Direction=DMA_PERIPH_TO_MEMORY;            //���赽�洢��
    UART2RxDMA_Handler.Init.PeriphInc=DMA_PINC_DISABLE;                //���������ģʽ
    UART2RxDMA_Handler.Init.MemInc=DMA_MINC_ENABLE;                    //�洢������ģʽ
    UART2RxDMA_Handler.Init.PeriphDataAlignment=DMA_PDATAALIGN_BYTE;   //�������ݳ���:8λ
    UART2RxDMA_Handler.Init.MemDataAlignment=DMA_MDATAALIGN_BYTE;      //�洢�����ݳ���:8/16/32λ
    UART2RxDMA_Handler.Init.Mode=DMA_CIRCULAR;                         //ʹ��ѭ��ģʽ 
    UART2RxDMA_Handler.Init.Priority=DMA_PRIORITY_HIGH;                //�����ȼ�
    UART2RxDMA_Handler.Init.FIFOMode=DMA_FIFOMODE_ENABLE;              //ʹ��FIFO
    UART2RxDMA_Handler.Init.FIFOThreshold=DMA_FIFO_THRESHOLD_HALFFULL; //ʹ��1/2��FIFO 
    UART2RxDMA_Handler.Init.MemBurst=DMA_MBURST_SINGLE;                //�洢��ͻ������
    UART2RxDMA_Handler.Init.PeriphBurst=DMA_PBURST_SINGLE;             //����ͻ�����δ��� 
    HAL_DMA_DeInit(&UART2RxDMA_Handler);                               //�������ǰ������
    HAL_DMA_Init(&UART2RxDMA_Handler);	                               //��ʼ��DMA
    
    //�ڿ���DMA֮ǰ��ʹ��__HAL_UNLOCK()����һ��DMA,��ΪHAL_DMA_Statrt()HAL_DMAEx_MultiBufferStart()
    //����������һ��ʼҪ��ʹ��__HAL_LOCK()����DMA,������__HAL_LOCK()���жϵ�ǰ��DMA״̬�Ƿ�Ϊ����״̬�������
    //����״̬�Ļ���ֱ�ӷ���HAL_BUSY�������ᵼ�º���HAL_DMA_Statrt()��HAL_DMAEx_MultiBufferStart()������DMA����
    //����ֱ�ӱ�������DMAҲ�Ͳ�������������Ϊ�˱���������������������DMA֮ǰ�ȵ���__HAL_UNLOC()�Ƚ���һ��DMA��
    __HAL_UNLOCK(&UART2RxDMA_Handler);
	
    if(mem1addr==0)    //����DMA����ʹ��˫����
    {
        HAL_DMA_Start(&UART2RxDMA_Handler,(u32)&USART2->DR,mem0addr,memsize);
    }
    else                //ʹ��˫����
    {
        HAL_DMAEx_MultiBufferStart(&UART2RxDMA_Handler,(u32)&USART2->DR,mem0addr,mem1addr,memsize);//����˫����
        __HAL_DMA_ENABLE_IT(&UART2RxDMA_Handler,DMA_IT_TC);		//������������ж�
        HAL_NVIC_SetPriority(DMA1_Stream5_IRQn,0,0);        	//DMA�ж����ȼ�
        HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);
    }
	
    __HAL_DMA_ENABLE(&UART2RxDMA_Handler); //ʹ��DMA
}



//USART2,��������
void USART2_Start(void)
{  
    __HAL_DMA_ENABLE(&UART2RxDMA_Handler); //ʹ��DMA
	
//	HAL_DMA_Start(UART2_Handle->hdmarx, (u32)RX_buf, (uint32_t)&UART2_Handle->Instance->DR, RxSize);//����DMA����
    
//	UART2_Handle->Instance->CR3 |= USART_CR3_DMAR;//ʹ�ܴ���DMA
}

//USART2,�رմ���
void USART2_Stop(void)
{ 
    __HAL_DMA_DISABLE(&UART2RxDMA_Handler);//�ر�DMA
} 


//DMA1������5�жϷ�����
void DMA1_Stream5_IRQHandler(void)
{
    if(__HAL_DMA_GET_FLAG(&UART2RxDMA_Handler,DMA_FLAG_TCIF1_5)!=RESET)//DMA�������
    {
        __HAL_DMA_CLEAR_FLAG(&UART2RxDMA_Handler,DMA_FLAG_TCIF1_5);//���DMA��������жϱ�־λ
		printf("DMA IT OK!\r\n");
    } 
}





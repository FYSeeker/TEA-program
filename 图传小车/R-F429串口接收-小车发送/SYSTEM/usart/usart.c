#include "usart.h"
#include "delay.h"
#include "lcd.h"

////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��os,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//os ʹ��	  
#endif


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
//#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)	
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 
}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
void _sys_exit(int x) 
{ 
	x = x; 
} 
//�ض���fputc���� 
int fputc(int ch, FILE *f)
{ 	
	while((USART1->SR&0X40)==0);//ѭ������,ֱ���������   
	USART1->DR = (u8) ch;      
	return ch;
}
#endif 

#if EN_USART1_RX   //���ʹ���˽���
//����1�жϷ������
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���   	
u8 USART_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.
//����״̬
//bit15��	������ɱ�־
//bit14��	���յ�0x0d
//bit13~0��	���յ�����Ч�ֽ���Ŀ
u16 USART_RX_STA=0;       //����״̬���	

u8 aRxBuffer[RXBUFFERSIZE];//HAL��ʹ�õĴ��ڽ��ջ���
UART_HandleTypeDef UART1_Handler; //UART���

//��ʼ��IO ����1 
//bound:������
void uart_init(u32 bound)
{	
	//UART ��ʼ������
	UART1_Handler.Instance=USART1;					    //USART1
	UART1_Handler.Init.BaudRate=bound;				    //������
	UART1_Handler.Init.WordLength=UART_WORDLENGTH_8B;   //�ֳ�Ϊ8λ���ݸ�ʽ
	UART1_Handler.Init.StopBits=UART_STOPBITS_1;	    //һ��ֹͣλ
	UART1_Handler.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART1_Handler.Init.HwFlowCtl=UART_HWCONTROL_NONE;   //��Ӳ������
	UART1_Handler.Init.Mode=UART_MODE_TX_RX;		    //�շ�ģʽ
	HAL_UART_Init(&UART1_Handler);					    //HAL_UART_Init()��ʹ��UART1
	
	//HAL_UART_Receive_IT(&UART1_Handler, (u8 *)aRxBuffer, RXBUFFERSIZE);//�ú����Ὺ�������жϣ���־λUART_IT_RXNE���������ý��ջ����Լ����ջ���������������(ʹ�ûص����������ж���Ҫ���øú�����
  
}

//UART�ײ��ʼ����ʱ��ʹ�ܣ��������ã��ж�����
//�˺����ᱻHAL_UART_Init()����
//huart:���ھ��

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	if(huart->Instance==USART1)//����Ǵ���1�����д���1 MSP��ʼ��
	{
		__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
		__HAL_RCC_USART1_CLK_ENABLE();			//ʹ��USART1ʱ��
	
		GPIO_Initure.Pin=GPIO_PIN_9;			//PA9
		GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
		GPIO_Initure.Pull=GPIO_PULLUP;			//����
		GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
		GPIO_Initure.Alternate=GPIO_AF7_USART1;	//����ΪUSART1
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA9

		GPIO_Initure.Pin=GPIO_PIN_10;			//PA10
		HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA10
		__HAL_UART_DISABLE_IT(huart,UART_IT_TC);
#if EN_USART1_RX
		__HAL_UART_ENABLE_IT(huart,UART_IT_RXNE);		//���������ж�
		HAL_NVIC_EnableIRQ(USART1_IRQn);				//ʹ��USART1�ж�ͨ��
		HAL_NVIC_SetPriority(USART1_IRQn,3,3);			//��ռ���ȼ�3�������ȼ�3
#endif	
	}

}


 


//����1�жϷ������
void USART1_IRQHandler(void)                	
{ 
	u8 Res;
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntEnter();    
#endif
	if((__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_RXNE)!=RESET))  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{
        HAL_UART_Receive(&UART1_Handler,&Res,1,1000); 
		if((USART_RX_STA&0x8000)==0)//����δ���
		{
			if(USART_RX_STA&0x4000)//���յ���0x0d
			{
				if(Res!=0x0a)USART_RX_STA=0;//���մ���,���¿�ʼ
				else USART_RX_STA|=0x8000;	//��������� 
			}
			else //��û�յ�0X0D
			{	
				if(Res==0x0d)USART_RX_STA|=0x4000;
				else
				{
					USART_RX_BUF[USART_RX_STA&0X3FFF]=Res ;
					USART_RX_STA++;
					if(USART_RX_STA>(USART_REC_LEN-1))USART_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
				}		 
			}
		}   		 
	}
	HAL_UART_IRQHandler(&UART1_Handler);	
#if SYSTEM_SUPPORT_OS	 	//ʹ��OS
	OSIntExit();  											 
#endif
} 
#endif	


void uart1_send_data(u16 data1, u16 data2, u16 data3, u16 data4)
{
	u8 TX_BUF[11];
	
	TX_BUF[0] = 0xff;
	TX_BUF[1] = data1>>8;
	TX_BUF[2] = data1&0x0f;
	TX_BUF[3] = data2>>8;
	TX_BUF[4] = data2&0x0f;
	TX_BUF[5] = data3>>8;
	TX_BUF[6] = data3&0x0f;
	TX_BUF[7] = data4>>8;
	TX_BUF[8] = data4&0x0f;
	TX_BUF[9] = '\r';
	TX_BUF[10] = '\n';
	
	HAL_UART_Transmit(&UART1_Handler,(uint8_t*)TX_BUF,11,1000);	//��������
	while(__HAL_UART_GET_FLAG(&UART1_Handler,UART_FLAG_TC)!=SET);		//�ȴ����ͽ���
}


/*********************************************************************/
//������Ӵ���

UART_HandleTypeDef UART2_Handle;	//����2���

/*����2��ʼ��*/
//bound:������
void MyUSART2_Init(u32 bound)
{	
    //GPIO�˿�����
	GPIO_InitTypeDef GPIO_Initure;
	
	__HAL_RCC_GPIOA_CLK_ENABLE();			//ʹ��GPIOAʱ��
	__HAL_RCC_USART2_CLK_ENABLE();			//ʹ��USART2ʱ��
	
	GPIO_Initure.Pin=GPIO_PIN_2;			//PA2
	GPIO_Initure.Mode=GPIO_MODE_AF_PP;		//�����������
	GPIO_Initure.Pull=GPIO_PULLUP;			//����
	GPIO_Initure.Speed=GPIO_SPEED_FAST;		//����
	GPIO_Initure.Alternate=GPIO_AF7_USART2;	//����ΪUSART2
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA2

	GPIO_Initure.Pin=GPIO_PIN_3;			//PA3
	HAL_GPIO_Init(GPIOA,&GPIO_Initure);	   	//��ʼ��PA3
		
	//UART ��ʼ������
	UART2_Handle.Instance=USART2;					    //USART2
	UART2_Handle.Init.BaudRate=bound;				    //������
	UART2_Handle.Init.WordLength=UART_WORDLENGTH_8B;	//�ֳ�Ϊ8λ���ݸ�ʽ
	UART2_Handle.Init.StopBits=UART_STOPBITS_1;			//һ��ֹͣλ
	UART2_Handle.Init.Parity=UART_PARITY_NONE;		    //����żУ��λ
	UART2_Handle.Init.HwFlowCtl=UART_HWCONTROL_NONE;	//��Ӳ������
	UART2_Handle.Init.Mode=UART_MODE_TX_RX;		    	//�շ�ģʽ
	HAL_UART_Init(&UART2_Handle);					    //HAL_UART_Init()��ʹ��UART1
	
	__HAL_UART_DISABLE_IT(&UART2_Handle,UART_IT_TC);	//�رշ����ж�
//	__HAL_UART_DISABLE_IT(&UART2_Handle,UART_IT_RXNE);	//�رս����ж�
	__HAL_UART_ENABLE_IT(&UART2_Handle,UART_IT_RXNE);	//���������ж�
	HAL_NVIC_EnableIRQ(USART2_IRQn);					//ʹ��USART2�ж�ͨ��
	HAL_NVIC_SetPriority(USART2_IRQn,0,1);				//��ռ���ȼ�2�������ȼ�2
}

/*�պ���*/
void My_NOP()
{}

u8 U2Rx_sta = 0;	//����2���ձ��
	
/*����BUF*/
u8 RX_buf[RxSize]={0};

//����2�жϷ������
void USART2_IRQHandler(void)                	
{ 
	static u16 i=0;
		
	if(USART2->SR & (1<<5))  //�����ж�
	{
		RX_buf[i] = USART2->DR;
		i++;
		
		if(i==RxSize)
		{	
			
			for(i=0;i<RxSize;i+=2)
			{
				LCD->LCD_RAM = (RX_buf[i]<<8)+RX_buf[i+1];
			}			
			i = 0;
			USART2->DR = 0xf0;
			while(!(USART2->SR & (1<<6)));	//�ȴ��������
		}
	}
	USART2->SR &= ~(1<<5);	//����жϱ�־λ
}




/*���ڷ�������*/
void MyUSART_SendData(u8* pbuf,u16 len)
{
	u16 x;
	for(x=0;x<len;x++)
	{
		USART2->DR = *pbuf;
		pbuf++;
		while(!(USART2->SR & (1<<6)));	//�ȴ��������
	}
}


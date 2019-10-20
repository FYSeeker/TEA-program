#include "sys.h"
#include "usart.h"	
#include "stdarg.h"
#include "stdio.h"
#include "string.h"
#include "blance.h"
////////////////////////////////////////////////////////////////////////////////// 	 
//���ʹ��ucos,����������ͷ�ļ�����.
#if SYSTEM_SUPPORT_OS
#include "includes.h"					//ucos ʹ��	  
#endif


//�������´���,֧��printf����,������Ҫѡ��use MicroLIB	  
#if 1
#pragma import(__use_no_semihosting)             
//��׼����Ҫ��֧�ֺ���                 
struct __FILE 
{ 
	int handle; 

}; 

FILE __stdout;       
//����_sys_exit()�Ա���ʹ�ð�����ģʽ    
_sys_exit(int x) 
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

/*ʹ��microLib�ķ���*/
 /* 
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);

	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) {}	
   
    return ch;
}
int GetKey (void)  { 

    while (!(USART1->SR & USART_FLAG_RXNE));

    return ((int)(USART1->DR & 0x1FF));
}
*/

USART_TypeDef* UARTN[UART_MAX]= {USART1, USART2, USART3, UART4, UART5}; //���崮�ں�

/***********************************************************************************************************
 *  @brief      I/O�ڳ�ʼ������
 *  @since      v5.0
 ***********************************************************************************************************/
void port_init(GPIO_TypeDef* GPIO_TX, uint16_t GPIO_Pin_TX, GPIO_TypeDef* GPIO_RX, uint16_t GPIO_Pin_RX, uint8_t  USART_IRQn)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//USART1_TX   GPIOA.9
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_TX; //PA.9
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	//�����������
	GPIO_Init(GPIO_TX, &GPIO_InitStructure);//��ʼ��GPIOA.9

	//USART1_RX	  GPIOA.10��ʼ��
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_RX;//PA10
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;//��������
	GPIO_Init(GPIO_RX, &GPIO_InitStructure);//��ʼ��GPIOA.10 

	//Usart1 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1 ;//��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;		//�����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);	//����ָ���Ĳ�����ʼ��VIC�Ĵ���
}
/***********************************************************************************************************
 *  @brief      ���ڳ�ʼ������
 *  @since      v5.0
 ***********************************************************************************************************/
void uart_init(UARTn_e uratn, u32 bound)
{
	USART_InitTypeDef USART_InitStructure;	

	switch(uratn)
	{
	case UAR1:
		//GPIO�˿�����
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1|RCC_APB2Periph_GPIOA, ENABLE);	//ʹ��USART1��GPIOAʱ��
		port_init(GPIOA,GPIO_Pin_9, GPIOA, GPIO_Pin_10,USART1_IRQn);	//��ʼ����Ӧ����
		break;
	case UAR2:
		//GPIO�˿�����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
		port_init(GPIOA,GPIO_Pin_2, GPIOA, GPIO_Pin_3,USART2_IRQn);	//��ʼ����Ӧ����
		break;
	case UAR3:
		//GPIO�˿�����
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);	//ʹ��USART1��GPIOAʱ��
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
		port_init(GPIOB,GPIO_Pin_10, GPIOB,GPIO_Pin_11,USART3_IRQn);	//��ʼ����Ӧ����
		break;
//	case UAR4:
//		//GPIO�˿�����
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART4, ENABLE);	//ʹ��USART1��GPIOAʱ��
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
//		port_init(GPIOC,GPIO_Pin_10, GPIOC,GPIO_Pin_11,UART4_IRQn);	//��ʼ����Ӧ����
//		break;
//	case UAR5:
//		//GPIO�˿�����
//		RCC_APB1PeriphClockCmd(RCC_APB1Periph_UART5, ENABLE);	//ʹ��USART1��GPIOAʱ��
//		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC|RCC_APB2Periph_GPIOD,ENABLE);
//		port_init(GPIOC,GPIO_Pin_12, GPIOD, GPIO_Pin_2,UART5_IRQn);	//��ʼ����Ӧ����
//		break;
	default:
        break;	
	}
	//USART ��ʼ������

	USART_InitStructure.USART_BaudRate = bound;//���ڲ�����
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;//�ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;//һ��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;//����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//��Ӳ������������
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;	//�շ�ģʽ

	USART_Init(UARTN[uratn], &USART_InitStructure); //��ʼ������uratn
	USART_ITConfig(UARTN[uratn], USART_IT_RXNE, ENABLE);//�������ڽ����ж�
	USART_Cmd(UARTN[uratn], ENABLE);                    //ʹ�ܴ���uratn 
}

/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
//ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ��� 

#if Open_UAR1   //������˴���1

u8 USART1_RX_BUF[USART_REC_LEN];     	//���ջ���,���USART_REC_LEN���ֽ�.   
u16 USART1_RX_STA=0;       				//����״̬��� bit15��	������ɱ�־
													//bit14��	���յ�0x0d
													//bit13~0��	���յ�����Ч�ֽ���Ŀ

/*****************************************************
 *  @brief      ����1�жϷ�����
 *  @since      v5.0
 *****************************************************/
void USART1_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART1);	//��ȡ���յ�������
		
		if((USART1_RX_STA&0x8000)==0)//����δ���
			{
			if(USART1_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART1_RX_STA=0;//���մ���,���¿�ʼ
				else USART1_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART1_RX_STA|=0x4000;
				else
					{
					USART1_RX_BUF[USART1_RX_STA&0X3FFF]=Res ;
					USART1_RX_STA++;
					if(USART1_RX_STA>(USART_REC_LEN-1))USART1_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*��������:	uart_init
*��������:	���ڳ�ʼ��
*��������: 	UARTn_e       	ģ���UAR1~UART5
			bound			������
*�� �� ֵ:	��
***********************************************************************/
#if Open_UAR2
u8 USART2_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.   
u16 USART2_RX_STA=0;       			//����״̬��� bit15��	������ɱ�־
												 //bit14��	���յ�0x0d
												 //bit13~0��	���յ�����Ч�ֽ���Ŀ

/********************************************
 *  @brief      ����2�жϷ�����
 *  @since      v5.0
 ********************************************/
void USART2_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART2);	//��ȡ���յ�������
		
		if((USART2_RX_STA&0x8000)==0)//����δ���
			{
			if(USART2_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART2_RX_STA=0;//���մ���,���¿�ʼ
				else USART2_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART2_RX_STA|=0x4000;
				else
					{
					USART2_RX_BUF[USART2_RX_STA&0X3FFF]=Res ;
					USART2_RX_STA++;
					if(USART2_RX_STA>(USART_REC_LEN-1))USART2_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 
	
/*****************************************************
 *  @brief      ����2��ӡ����
 *  @since      v5.0
 *****************************************************/
void u2_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART2_TX_BUF[50]; 	//���巢�����飬�淢������
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART2_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART2_TX_BUF);
	for(j=0;j<i;j++)
	{
		USART2->DR=USART2_TX_BUF[j];//�Ѵ���1�����ݷ��͵�����2
			while((USART2->SR&0X40)==0);//�ȴ����ͽ���
	}
}
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*��������:	uart_init
*��������:	���ڳ�ʼ��
*��������: 	UARTn_e       	ģ���UAR1~UART5
			bound			������
*�� �� ֵ:	��
***********************************************************************/
#if Open_UAR3
u8 USART3_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.   
u16 USART3_RX_STA=0;       			//����״̬��� bit15��	������ɱ�־
												 //bit14��	���յ�0x0d
												 //bit13~0��	���յ�����Ч�ֽ���Ŀ

/********************************************
 *  @brief      ����3�жϷ�����
 *  @since      v5.0
 ********************************************/
void USART3_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(USART3);	//��ȡ���յ�������
		
		if((USART3_RX_STA&0x8000)==0)//����δ���
			{
			if(USART3_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART3_RX_STA=0;//���մ���,���¿�ʼ
				else USART3_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART3_RX_STA|=0x4000;
				else
					{
					USART3_RX_BUF[USART3_RX_STA&0X3FFF]=Res ;
					USART3_RX_STA++;
					if(USART3_RX_STA>(USART_REC_LEN-1))USART3_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 


/*****************************************************
 *  @brief      ����3��ӡ����
 *  @since      v5.0
 *****************************************************/
void u3_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART3_TX_BUF[50]; 	//���巢�����飬�淢������
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART3_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART3_TX_BUF);
	for(j=0;j<i;j++)
	{
		USART3->DR=USART3_TX_BUF[j];//�Ѵ���1�����ݷ��͵�����2
			while((USART3->SR&0X40)==0);//�ȴ����ͽ���
	}
}
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*��������:	uart_init
*��������:	���ڳ�ʼ��
*��������: 	UARTn_e       	ģ���UAR1~UART5
			bound			������
*�� �� ֵ:	��
***********************************************************************/
#if Open_UAR4
u8 USART4_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.   
u16 USART4_RX_STA=0;       			//����״̬��� bit15��	������ɱ�־
												 //bit14��	���յ�0x0d
												 //bit13~0��	���յ�����Ч�ֽ���Ŀ

/********************************************
 *  @brief      ����4�жϷ�����
 *  @since      v5.0
 ********************************************/
void UART4_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART4, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(UART4);	//��ȡ���յ�������
		
		if((USART4_RX_STA&0x8000)==0)//����δ���
			{
			if(USART4_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART4_RX_STA=0;//���մ���,���¿�ʼ
				else USART4_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART4_RX_STA|=0x4000;
				else
					{
					USART4_RX_BUF[USART4_RX_STA&0X3FFF]=Res ;
					USART4_RX_STA++;
					if(USART4_RX_STA>(USART_REC_LEN-1))USART4_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 


/*****************************************************
 *  @brief      ����4��ӡ����
 *  @since      v5.0
 *****************************************************/
void u4_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART4_TX_BUF[50]; 	//���巢�����飬�淢������
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART4_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART4_TX_BUF);
	for(j=0;j<i;j++)
	{
		UART4->DR=USART4_TX_BUF[j];//�Ѵ���1�����ݷ��͵�����2
			while((UART4->SR&0X40)==0);//�ȴ����ͽ���
	}
}
#endif


/***************************************************************************************************
 *  @brief      **********************************************************************************
 *  @since      *********************************************************************************
 **************************************************************************************************/
/**********************************************************************
*��������:	uart_init
*��������:	���ڳ�ʼ��
*��������: 	UARTn_e       	ģ���UAR1~UART5
			bound			������
*�� �� ֵ:	��
***********************************************************************/
#if Open_UAR5
u8 USART5_RX_BUF[USART_REC_LEN];     //���ջ���,���USART_REC_LEN���ֽ�.   
u16 USART5_RX_STA=0;       			//����״̬��� bit15��	������ɱ�־
												 //bit14��	���յ�0x0d
												 //bit13~0��	���յ�����Ч�ֽ���Ŀ

/********************************************
 *  @brief      ����4�жϷ�����
 *  @since      v5.0
 ********************************************/
void UART5_IRQHandler(void)                	//����1�жϷ������
	{
	u8 Res;
#if SYSTEM_SUPPORT_OS 		//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntEnter();    
#endif
	if(USART_GetITStatus(UART5, USART_IT_RXNE) != RESET)  //�����ж�(���յ������ݱ�����0x0d 0x0a��β)
		{
		Res =USART_ReceiveData(UART5);	//��ȡ���յ�������
		
		if((USART5_RX_STA&0x8000)==0)//����δ���
			{
			if(USART5_RX_STA&0x4000)//���յ���0x0d
				{
				if(Res!=0x0a)USART5_RX_STA=0;//���մ���,���¿�ʼ
				else USART5_RX_STA|=0x8000;	//��������� 
				}
			else //��û�յ�0X0D
				{	
				if(Res==0x0d)USART5_RX_STA|=0x4000;
				else
					{
					USART5_RX_BUF[USART5_RX_STA&0X3FFF]=Res ;
					USART5_RX_STA++;
					if(USART5_RX_STA>(USART_REC_LEN-1))USART5_RX_STA=0;//�������ݴ���,���¿�ʼ����	  
					}		 
				}
			}   		 
     } 
#if SYSTEM_SUPPORT_OS 	//���SYSTEM_SUPPORT_OSΪ�棬����Ҫ֧��OS.
	OSIntExit();  											 
#endif
} 


/*****************************************************
 *  @brief      ����4��ӡ����
 *  @since      v5.0
 *****************************************************/
void u5_printf(char* fmt,...)
{
	u16 i,j;
	static u8 USART5_TX_BUF[50]; 	//���巢�����飬�淢������
	va_list ap;
	va_start(ap,fmt);
	vsprintf((char*)USART5_TX_BUF,fmt,ap);
	va_end(ap);
	i=strlen((const char*)USART5_TX_BUF);
	for(j=0;j<i;j++)
	{
		UART5->DR=USART5_TX_BUF[j];//�Ѵ���1�����ݷ��͵�����2
			while((UART5->SR&0X40)==0);//�ȴ����ͽ���
	}
}
#endif

/*m^n����������ֵ:m^n�η�*/
double Usart_Pow(double m,u8 n)
{
	double result=1;	 
	while(n--) result*=m;    
	return result;
}

/**********************************************************************
*��������:	Uart_DealDate
*��������:	���ڴ�������
*��������: 	USARTx_RX_STA	����״̬���
*�� �� ֵ:	��
***********************************************************************/

void Uart_DealDate()
{	 
	u8 i,n,digit=0,scope=0;
	u8 usart_flag=0,point_flag=0;
	u8 x=0;
	u8 x1=0,x2=0,x3=0,x4=0,x5=0;
	u8 x6=0,x7=0,x8=0,x9=0,x10=0,x11=0;
	u8 buf_1[]="P1:";
	u8 buf_2[]="I1:";
	u8 buf_3[]="D1:";
	u8 buf_4[]="P2:";
	u8 buf_5[]="I2:";
	u8 buf_6[]="D2:";
	u8 buf_7[]="EN:";
	u8 buf_8[]="SS:";
	u8 buf_9[]="XX:";
	u8 buf_10[]="XX:";
	u8 buf_11[]="XX:";
	double temp=0;
	
	if((USART1_RX_STA&0x8000)==0x8000)	//���ݽ������
	{
		/*�ж����ݸ�ʽ*/
		for(i=0;i<3;i++)		
		{
			if(USART1_RX_BUF[i]==buf_1[i])  x1++;
			if(USART1_RX_BUF[i]==buf_2[i])  x2++;
			if(USART1_RX_BUF[i]==buf_3[i])  x3++;
			if(USART1_RX_BUF[i]==buf_4[i])  x4++;
			if(USART1_RX_BUF[i]==buf_5[i])  x5++;
			if(USART1_RX_BUF[i]==buf_6[i])  x6++;
			if(USART1_RX_BUF[i]==buf_7[i])  x7++;
			if(USART1_RX_BUF[i]==buf_8[i])  x8++;
			if(USART1_RX_BUF[i]==buf_9[i])  x9++;
			if(USART1_RX_BUF[i]==buf_10[i]) x10++;
			if(USART1_RX_BUF[i]==buf_11[i]) x11++;
		}
		
		if(x1==3||x2==3||x3==3||x4==3||x5==3||x6==3||x7==3||x8==3||x9==3||x10==3||x11==3)	//��ʽ��ȷ,��������
		{
			if(x1==3)
			{
				usart_flag = 1;
				printf("P1: ");
			}	
			else if(x2==3)
			{
				usart_flag = 2;
				printf("I1: ");
			}
			else if(x3==3)
			{
				usart_flag = 3;
				printf("D1: ");
			}
			else if(x4==3)
			{
				usart_flag = 4;
				printf("P2: ");
			}
			else if(x5==3)
			{
				usart_flag = 5;
				printf("I2: ");
			}
			else if(x6==3)
			{
				usart_flag = 6;
				printf("D2: ");
			}
			else if(x7==3)
			{
				usart_flag = 7;
				printf("EN: ");
			}
			else if(x8==3)
			{
				usart_flag = 8;
				printf("SS: ");
			}
			else if(x9==3)
			{
				usart_flag = 9;
				printf("XX: ");
			}
			else if(x10==3)
			{
				usart_flag = 10;
				printf("��ʽ��ȷ��");
			}
			else if(x11==3)
			{
				usart_flag = 11;
				printf("��ʽ��ȷ��");
			}
			
			/*�ж����ݷ�Χ*/
			i = 3;
			while(USART1_RX_BUF[i]!='\0')
			{
				if((USART1_RX_BUF[i]>='0'&&USART1_RX_BUF[i]<='9') || USART1_RX_BUF[i]=='.')
				{
					x++;
				}
				i++;
			}
			if(x+3==i)	//��Χ����
			{
				scope = 1;		//��Ƿ�Χ����
			}
			else
			{
				printf("�����쳣,�����·��ͣ�����\r\n");
				usart_flag = 0;
			}
			
			/*��������*/
			if(scope)
			{
				i = 3;
				while(USART1_RX_BUF[i]!='\0')
				{
					if(USART1_RX_BUF[i]=='.')  point_flag = i;	//���С���㣨�ӱ�ʾ��
					i++;
				}
				digit = i;		//��¼λ�����ӱ�ʾ��
				
				if(point_flag!=0)	//��С����
				{
					/*������������*/
					n = point_flag-1-3;
					temp = 0;
					for(i=3;i<point_flag;i++)	
					{
						temp += (USART1_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					/*����С������*/
					n = 1;
					for(i=point_flag+1;i<digit;i++)
					{
						temp += (USART1_RX_BUF[i]-'0') * Usart_Pow(0.1,n++);
					}
					printf("%lf\t\r\n",temp);
				}
				else	//û��С����
				{
					n = digit-1-3;
					temp = 0;
					for(i=3;i<digit;i++)	
					{
						temp += (USART1_RX_BUF[i]-'0')*Usart_Pow(10,n--);	
					}
					printf("%lf\t\r\n",temp);	
				}	
			}
			
			switch(usart_flag)
			{
				/*PID����*/
				case 1:BlancePid.P = temp;printf("BlancePid.P = %f\r\n",BlancePid.P);
					break;
				case 2:BlancePid.I = temp;printf("BlancePid.I = %f\r\n",BlancePid.I);
					break;
				case 3:BlancePid.D = temp;printf("BlancePid.D = %f\r\n",BlancePid.D);
					break;
				
				case 7:ARMED = temp;printf("ARMED = %d\r\n",ARMED);
					break;
				case 8:Accelerator = temp;printf("Accelerator = %d\r\n",Accelerator);
					break;
				
				default:break;
			}
		}
		else  printf("��ʽ���������·��ͣ�����\r\n");
		
		printf("\r\n");
		
		/*�������BUF*/
		i=0;
		while(USART1_RX_BUF[i]!='\0')
		{
			USART1_RX_BUF[i] = 0;
			i++;
		}
		USART1_RX_STA = 0;		//����״̬������λ����0
	}
}

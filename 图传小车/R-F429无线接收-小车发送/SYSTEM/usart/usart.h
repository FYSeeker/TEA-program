#ifndef _USART_H
#define _USART_H

#include "sys.h"
#include "stdio.h"	


/*���ݽ���BUF��С*/
#define RxSize 640*60	
	
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_REC_LEN  			200  	//�����������ֽ��� 200
#define EN_USART1_RX 			1		//ʹ�ܣ�1��/��ֹ��0������1����
	  	
extern u8  USART_RX_BUF[USART_REC_LEN]; //���ջ���,���USART_REC_LEN���ֽ�.ĩ�ֽ�Ϊ���з� 
extern u16 USART_RX_STA;         		//����״̬���	
extern UART_HandleTypeDef UART1_Handler; //UART���

#define RXBUFFERSIZE   1 //�����С
extern u8 aRxBuffer[RXBUFFERSIZE];//HAL��USART����Buffer

//����봮���жϽ��գ��벻Ҫע�����º궨��
void uart_init(u32 bound);

void uart1_send_data(u16 data1, u16 data2, u16 data3, u16 data4);


/*����2��ʼ��*/
//bound:������
void MyUSART2_Init(u32 bound);


/*���ڷ�������*/
void MyUSART_SendBit(u8 dat);

/*���ڷ�������*/
void MyUSART_SendData(u8* pbuf,u16 len);

void My_NOP(void);

#endif

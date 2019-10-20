#ifndef __24L01_H
#define __24L01_H
#include "sys.h"



/*-- �û��Զ���IO --*/
#define RCC_CE      			RCC_APB2Periph_GPIOB
#define GPIO_CE_PORT  			GPIOB    
#define GPIO_CE_PIN       		GPIO_Pin_7

#define RCC_CSN         		RCC_APB2Periph_GPIOB
#define GPIO_CSN_PORT   		GPIOB    
#define GPIO_CSN_PIN          	GPIO_Pin_8

#define RCC_IRQ             	RCC_APB2Periph_GPIOB
#define GPIO_IRQ_PORT       	GPIOB  
#define GPIO_IRQ_PIN      		GPIO_Pin_4

//24L01������
#define NRF24L01_CE   PBout(7) //24L01Ƭѡ�ź�
#define NRF24L01_CSN  PBout(8) //SPIƬѡ�ź�	   
#define NRF24L01_IRQ  PBin(4)  //IRQ������������
//24L01���ͽ������ݿ�ȶ���
#define TX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define RX_ADR_WIDTH    5   	//5�ֽڵĵ�ַ���
#define TX_PLOAD_WIDTH  3  		//3�ֽڵ��û����ݿ��
#define RX_PLOAD_WIDTH  3  		//3�ֽڵ��û����ݿ��
									   	   

void NRF24L01_Init(void);//��ʼ��
void NRF24L01_RX_Mode(void);//����Ϊ����ģʽ
void NRF24L01_TX_Mode(void);//����Ϊ����ģʽ
u8 NRF24L01_Write_Buf(u8 reg, u8 *pBuf, u8 u8s);//д������
u8 NRF24L01_Read_Buf(u8 reg, u8 *pBuf, u8 u8s);//��������		  
u8 NRF24L01_Read_Reg(u8 reg);			//���Ĵ���
u8 NRF24L01_Write_Reg(u8 reg, u8 value);//д�Ĵ���
u8 NRF24L01_Check(void);//���24L01�Ƿ����
u8 NRF24L01_TxPacket(u8 *txbuf);//����һ����������
u8 NRF24L01_RxPacket(u8 *rxbuf);//����һ����������

void Remotecontrol(void);

extern u8 ARMED;
extern s8 Turn_pwm;


#endif



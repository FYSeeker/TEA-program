#ifndef NRF24L01_H_
#define NRF24L01_H_

#include <reg52.h>

/************************************�û�����************************************/
#define ADDR_WIDTH		5		//����,���յ�ַ���
#define DATA_WIDTH		3		//����,�������ݵĳ���(�ֽ�,���Ϊ32)
/********************************************************************************/

/************************************�ⲿ����************************************/
#define CHECK_SUCCEED	0		//NRF20L01����
#define	CHECK_ERROR		1		//NRF20L01������
#define TX_SUCCEED		0		//���ͳɹ�
#define TX_ERROR		1		//����ʧ��
#define RX_SUCCEED		0		//���ճɹ�
#define RX_ERROR		1		//����ʧ��
/********************************************************************************/

sbit NRF24L01_CE   = P0^2;		//оƬģʽ������
sbit NRF24L01_SCK  = P0^3;		//оƬ����ʱ����(SPIʱ��)
sbit NRF24L01_MISO = P0^4;		//оƬ����������(����ӳ�)
sbit NRF24L01_IRQ  = P0^5;		//�ж��ź�			
sbit NRF24L01_MOSI = P0^6;		//оƬ����������(��������)
sbit NRF24L01_CSN  = P0^7;		//оƬƬѡ��,CSNΪ�͵�ƽоƬ����

/**********************************************************************
*��������:	Nrf24l01Init
*��������:	��ʼ��NRF24L01
*��������: 	��
*�� �� ֵ:	��
***********************************************************************/
void Nrf24l01Init(void);

/**********************************************************************
*��������:	CheckNrf24l01
*��������:	���NRF24L01ģ���Ƿ����
*��������: 	��
*�� �� ֵ:	CHECK_SUCCEED:	NRF24L01����
*			CHECK_ERROR:	NRF24L01������
***********************************************************************/
unsigned char CheckNrf24l01(void);

/**********************************************************************
*��������:	Nrf24l01SetTxMode
*��������:	����NRF24L01Ϊ����ģʽ
*��������: 	��
*�� �� ֵ:	��
***********************************************************************/
void Nrf24l01SetTXMode(void);

/**********************************************************************
*��������:	Nrf24l01SetRxMode
*��������:	����NRF24L01Ϊ����ģʽ
*��������: 	��
*�� �� ֵ:	��
***********************************************************************/
void Nrf24l01SetRXMode(void);

/**********************************************************************
*��������:	Nrf24l01TxPacket
*��������:	����NRF24L01��������
*��������: 	txBuf:		ָ��Ҫ���͵����ݵ��׵�ַ
*�� �� ֵ:	TX_SUCCEED:	�������
*			TX_ERROR:	����ʧ��
***********************************************************************/
unsigned char Nrf24l01TXPacket(unsigned char *txBuf);

/**********************************************************************
*��������:	Nrf24l01RXPacket
*��������:	����NRF24L01��������
*��������: 	rxBuf:	ָ��Ҫ���յ����ݵ��׵�ַ
*�� �� ֵ:	RX_SUCCEED:	�������
*			RX_ERROR:	����ʧ��
***********************************************************************/
unsigned char Nrf24l01RXPacket(unsigned char *rxBuf);



void Nrf24l01DelayUs(unsigned int x);
unsigned char SpiReadAndWrite(unsigned char dat);
unsigned char Nrf24l01WriteReg(const unsigned char reg, const unsigned char dat);
unsigned char Nrf24l01ReadReg(const unsigned char reg);
unsigned char Nrf24l01WriteBuf(const unsigned char reg, const unsigned char *pBuf, const unsigned char len);
unsigned char Nrf24l01ReadBuf(const unsigned char reg, unsigned char *pBuf, const unsigned char len);



#endif

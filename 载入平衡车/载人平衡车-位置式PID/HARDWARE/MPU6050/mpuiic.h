#ifndef __MPUIIC_H
#define __MPUIIC_H
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK MiniSTM32F103������
//MPU6050 IIC���� ����	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/4/18
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
	   		   
//IO��������
#define MPU1_SDA_IN()  {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=8<<12;}
#define MPU1_SDA_OUT() {GPIOA->CRH&=0XFFFF0FFF;GPIOA->CRH|=3<<12;}

//IO��������	 
#define MPU1_IIC_SCL    PAout(12) 		//SCL
#define MPU1_IIC_SDA    PAout(11) 		//SDA	 
#define MPU1_READ_SDA   PAin(11) 		//����SDA 
///////////////////////////////////////////////////////
//IO��������
#define MPU2_SDA_IN()  {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=8<<16;}
#define MPU2_SDA_OUT() {GPIOA->CRL&=0XFFF0FFFF;GPIOA->CRL|=3<<16;}

//IO��������	 
#define MPU2_IIC_SCL    PAout(5) 		//SCL
#define MPU2_IIC_SDA    PAout(4) 		//SDA	 
#define MPU2_READ_SDA   PAin(4) 		//����SDA 

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��

void MPU_IIC_Start(u8 witch);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(u8 witch);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd, u8 witch);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack, u8 witch);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(u8 witch); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(u8 witch);					//IIC����ACK�ź�
void MPU_IIC_NAck(u8 witch);				//IIC������ACK�ź�


//void IMPU_IC_Write_One_Byte(u8 daddr,u8 addr,u8 data);
//u8 MPU_IIC_Read_One_Byte(u8 daddr,u8 addr);	  
#endif

















#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "sys.h"
#include "usart.h"

/*I2C���IO���趨��*/
#define I2C_SDA_RCC		RCC_APB2Periph_GPIOB
#define I2C_SDA_GPIO	GPIOB
#define I2C_SDA_PIN		GPIO_Pin_10

#define I2C_SCL_RCC		RCC_APB2Periph_GPIOB
#define I2C_SCL_GPIO	GPIOB
#define I2C_SCL_PIN		GPIO_Pin_11

//IO��������
#define MPU_SDA_IN()  {GPIOB->CRH&=0XFFFFF0FF; GPIOB->CRH|=0x00000800;}
#define MPU_SDA_OUT() {GPIOB->CRH&=0XFFFFF0FF; GPIOB->CRH|=0x00000300;}	//PC10

//IO��������	 
#define MPU_IIC_SCL    PBout(11) 		//SCL
#define MPU_IIC_SDA    PBout(10) 		//SDA	 
#define MPU_READ_SDA   PBin(10) 		//����SDA

//IIC���в�������
void MPU_IIC_Delay(void);				//MPU IIC��ʱ����
void MPU_IIC_Init(void);                //��ʼ��IIC��IO��				 
void MPU_IIC_Start(void);				//����IIC��ʼ�ź�
void MPU_IIC_Stop(void);	  			//����IICֹͣ�ź�
void MPU_IIC_Send_Byte(u8 txd);			//IIC����һ���ֽ�
u8 MPU_IIC_Read_Byte(unsigned char ack);//IIC��ȡһ���ֽ�
u8 MPU_IIC_Wait_Ack(void); 				//IIC�ȴ�ACK�ź�
void MPU_IIC_Ack(void);					//IIC����ACK�ź�
void MPU_IIC_NAck(void);				//IIC������ACK�ź�

void IMPU_IC_Write_One_Byte(u8 daddr, u8 addr, u8 data);
u8 MPU_IIC_Read_One_Byte(u8 daddr, u8 addr);

#endif

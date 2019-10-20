#ifndef __MPUIIC_H
#define __MPUIIC_H

#include "sys.h"
#include "usart.h"

/*I2C���IO���趨��*/
#define I2C_SDA_RCC		RCC_APB2Periph_GPIOC
#define I2C_SDA_GPIO	GPIOC
#define I2C_SDA_PIN		GPIO_Pin_13

#define I2C_SCL_RCC		RCC_APB2Periph_GPIOC 
#define I2C_SCL_GPIO	GPIOC 
#define I2C_SCL_PIN		GPIO_Pin_14

//IO��������
#define MPU_SDA_IN()  {GPIOC->CRH&=0XFF0FFFFF; GPIOC->CRH|=0x00800000;}
#define MPU_SDA_OUT() {GPIOC->CRH&=0XFF0FFFFF; GPIOC->CRH|=0x00300000;}	//PC13

//IO��������	 
#define MPU_IIC_SCL    PCout(14) 		//SCL
#define MPU_IIC_SDA    PCout(13) 		//SDA	 
#define MPU_READ_SDA   PCin(13) 		//����SDA

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

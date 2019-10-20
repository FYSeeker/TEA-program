#include "mpuiic.h"
#include "delay.h"
 
  //MPU IIC ��ʱ����
void MPU_IIC_Delay(void)
{
	delay_us(2);
}

//��ʼ��IIC
void MPU_IIC_Init(void)
{					     
  GPIO_InitTypeDef  GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//��ʹ������IO PORTCʱ�� 
		
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5;	 // �˿�����
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
  GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIO 
	
  GPIO_SetBits(GPIOA,GPIO_Pin_12|GPIO_Pin_11|GPIO_Pin_4|GPIO_Pin_5);						 //PB10,PB11 �����	
 
}
//����IIC��ʼ�ź�
void MPU_IIC_Start(u8 witch)
{
	if(witch == 1)
	{
		MPU1_SDA_OUT();     //sda�����
		MPU1_IIC_SDA=1;	  	  
		MPU1_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU1_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
		MPU_IIC_Delay();
		MPU1_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	}else if(witch == 2)
	{
		MPU2_SDA_OUT();     //sda�����
		MPU2_IIC_SDA=1;	  	  
		MPU2_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU2_IIC_SDA=0;//START:when CLK is high,DATA change form high to low 
		MPU_IIC_Delay();
		MPU2_IIC_SCL=0;//ǯסI2C���ߣ�׼�����ͻ�������� 
	}
	
}	  
//����IICֹͣ�ź�
void MPU_IIC_Stop(u8 witch)
{
	if(witch == 1)
	{
		MPU1_SDA_OUT();//sda�����
		MPU1_IIC_SCL=0;
		MPU1_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		MPU_IIC_Delay();
		MPU1_IIC_SCL=1;  
		MPU1_IIC_SDA=1;//����I2C���߽����ź�
		MPU_IIC_Delay();
	}else if(witch == 2)
	{	
		MPU2_SDA_OUT();//sda�����
		MPU2_IIC_SCL=0;
		MPU2_IIC_SDA=0;//STOP:when CLK is high DATA change form low to high
		MPU_IIC_Delay();
		MPU2_IIC_SCL=1;  
		MPU2_IIC_SDA=1;//����I2C���߽����ź�
		MPU_IIC_Delay();
	}
								   	
}
//�ȴ�Ӧ���źŵ���
//����ֵ��1������Ӧ��ʧ��
//        0������Ӧ��ɹ�
u8 MPU_IIC_Wait_Ack(u8 witch)
{
	u8 ucErrTime=0;
	if(witch == 1)
	{
		MPU1_SDA_IN();      //SDA����Ϊ����  
		MPU1_IIC_SDA=1;MPU_IIC_Delay();	   
		MPU1_IIC_SCL=1;MPU_IIC_Delay();	 
		while(MPU1_READ_SDA)
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				MPU_IIC_Stop(1);
				return 1;
			}
		}
		MPU1_IIC_SCL=0;//ʱ�����0 	   
		return 0; 
		
	}else if(witch == 2)
	{
		MPU2_SDA_IN();      //SDA����Ϊ����  
		MPU2_IIC_SDA=1;MPU_IIC_Delay();	   
		MPU2_IIC_SCL=1;MPU_IIC_Delay();	 
		while(MPU2_READ_SDA)
		{
			ucErrTime++;
			if(ucErrTime>250)
			{
				MPU_IIC_Stop(2);
				return 1;
			}
		}
		MPU2_IIC_SCL=0;//ʱ�����0 	   
		return 0;  
	}	 
} 
//����ACKӦ��
void MPU_IIC_Ack(u8 witch)
{
	if(witch == 1)
	{
		MPU1_IIC_SCL=0;
		MPU1_SDA_OUT();
		MPU1_IIC_SDA=0;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=0;
	}else if(witch == 2)
	{
		MPU2_IIC_SCL=0;
		MPU2_SDA_OUT();
		MPU2_IIC_SDA=0;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=0;
	}
	
}
//������ACKӦ��		    
void MPU_IIC_NAck(u8 witch)
{
	if(witch == 1)
	{
		MPU1_IIC_SCL=0;
		MPU1_SDA_OUT();
		MPU1_IIC_SDA=1;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU1_IIC_SCL=0;
	}else if(witch == 2)
	{
		MPU2_IIC_SCL=0;
		MPU2_SDA_OUT();
		MPU2_IIC_SDA=1;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=1;
		MPU_IIC_Delay();
		MPU2_IIC_SCL=0;
	}
	
}					 				     
//IIC����һ���ֽ�
//���شӻ�����Ӧ��
//1����Ӧ��
//0����Ӧ��			  
void MPU_IIC_Send_Byte(u8 txd, u8 witch)
{  
	u8 t; 
	if(witch == 1)
	{  
		MPU1_SDA_OUT(); 	    
		MPU1_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
		for(t=0;t<8;t++)
		{              
			MPU1_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			MPU1_IIC_SCL=1;
			MPU_IIC_Delay(); 
			MPU1_IIC_SCL=0;	
			MPU_IIC_Delay();
		}
	}else if(witch == 2)
	{   
		MPU2_SDA_OUT(); 	    
		MPU2_IIC_SCL=0;//����ʱ�ӿ�ʼ���ݴ���
		for(t=0;t<8;t++)
		{              
			MPU2_IIC_SDA=(txd&0x80)>>7;
			txd<<=1; 	  
			MPU2_IIC_SCL=1;
			MPU_IIC_Delay(); 
			MPU2_IIC_SCL=0;	
			MPU_IIC_Delay();
		}	
	}
    	 
} 	    
//��1���ֽڣ�ack=1ʱ������ACK��ack=0������nACK   
u8 MPU_IIC_Read_Byte(unsigned char ack, u8 witch)
{
	unsigned char i,receive=0;
	if(witch == 1)
	{
		MPU1_SDA_IN();//SDA����Ϊ����
		for(i=0;i<8;i++ )
		{
			MPU1_IIC_SCL=0; 
			MPU_IIC_Delay();
			MPU1_IIC_SCL=1;
			receive<<=1;
			if(MPU1_READ_SDA)receive++;   
			MPU_IIC_Delay(); 
		}					 
		if (!ack)
			MPU_IIC_NAck(1);//����nACK
		else
			MPU_IIC_Ack(1); //����ACK   
		return receive;
	}else if(witch == 2)
	{
		MPU2_SDA_IN();//SDA����Ϊ����
		for(i=0;i<8;i++ )
		{
			MPU2_IIC_SCL=0; 
			MPU_IIC_Delay();
			MPU2_IIC_SCL=1;
			receive<<=1;
			if(MPU2_READ_SDA)receive++;   
			MPU_IIC_Delay(); 
		}					 
		if (!ack)
			MPU_IIC_NAck(2);//����nACK
		else
			MPU_IIC_Ack(2); //����ACK   
		return receive;
	}
	
}


















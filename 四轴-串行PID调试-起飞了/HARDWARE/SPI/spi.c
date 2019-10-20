#include "spi.h"
#include "delay.h"


/*******************************
*����˵����
*1--SDA <-----> 
*2--SCK <----->PB5 
*3--MOSI<----->PB6 
*4--MISO<----->PB3 
************************************/
void SIMUL_SPI_Init(void)  //ģ��SPI IO�ڳ�ʼ��
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);//ʱ��ʹ��

	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5|GPIO_Pin_6;		//SCK,MOSI����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			//�ٶ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//�������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;        			//MISO����ܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      		//�ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//��ͨ����              
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}   


//SPIx ��дһ���ֽ�
//TxData:Ҫд����ֽ�
//����ֵ:��ȡ�����ֽ�
u8 SPI1_ReadWriteByte(u8 Byte)
{		
	u8 bit_ctr;
	for(bit_ctr=0;bit_ctr<8;bit_ctr++)
	{
		if(Byte &0x80)                   	//forѭ���ж����ݽ������ƣ���ÿ�ζ�ȡ���λ.�����ζ�ȡ����
			MOSI_H;							//����λΪ1����1
		else
			MOSI_L;                     	//����λΪ0����0
		SCK_H;								//SCK�ø߷�������
		Byte <<= 1;                    		//����������ƣ�׼����һλ���ݶ�ȡ�����ճ�һλ׼����ȡ����
		if(MISO_Read)						//��ȡMISO�ĵ�ƽ�������ݣ�
			Byte |= 0x01;
		SCK_L;                          	//�õͣ������������ݶ�ȡ 
	}
	return Byte;				    
}




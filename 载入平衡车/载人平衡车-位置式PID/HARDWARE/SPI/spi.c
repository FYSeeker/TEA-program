#include "spi.h"
#include "delay.h"


//ģ��SPI IO�ڳ�ʼ��
void SIMUL_SPI_Init(void)  
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_SCLK|RCC_MOSI|RCC_MISO|RCC_APB2Periph_AFIO, ENABLE);//ʱ��ʹ��
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag��ʹ��SWD��������SWDģʽ����

	GPIO_InitStructure.GPIO_Pin =  GPIO_SCLK_PIN;				//SCK����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			//�ٶ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//�������
	GPIO_Init(GPIO_SCLK_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin =  GPIO_MOSI_PIN;				//MOSI����
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 			//�ٶ�
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 			//�������
	GPIO_Init(GPIO_MOSI_PORT, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_MISO_PIN;        		//MISO����ܽ�
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;      		//�ٶ�
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;				//��ͨ����              
	GPIO_Init(GPIO_MISO_PORT, &GPIO_InitStructure);
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




#include "main.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	LED_Init();                	// LED��ʼ��
	KEY_Init();				   	// ������ʼ��
	delay_init();	    	   	// ��ʱ������ʼ��
	uart_init(115200);		   	// ���ڳ�ʼ��
	
//	MPU_Init();					// ��ʼ��MPU6050
//	while(mpu_dmp_init())		// һ��Ҫ��ʼ���ɹ�
//	{LED0_Flash(666);}
	
	NRF24L01_Init();    	  	// ��ʼ��NRF24L01 
	while(NRF24L01_Check()==1)  // ���NRF24L01
	{
		LED0_Flash(666);
	}
	LED0=1;
	NRF24L01_TX_Mode();			// ����Ϊ����ģʽ 
	
	OLED_IIC_Init();			// ��ʼ��OLEDIIC
	
	Adc_Init();					// ADC��ʼ��
	
	OLED_Basic();				// ��������
	
	while(1)
	{	
		ADC_Gather();			// ҡ�����ݲɼ�
		KEY_Control();			// ��������
		NRF_SendData();			// �������ݷ���
		OLED_Display();			// OLED��ʾ����
	} 
}




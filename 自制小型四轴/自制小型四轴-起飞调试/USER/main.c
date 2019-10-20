#include "main.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�����ж����ȼ�����2
	delay_init();					//��ʱ������ʼ��
	uart_init(9600);				//���ڳ�ʼ��Ϊ9600
	LED_Init();						//��ʼ��LED
//	KEY_Init();						//��ʼ������
	
	NRF24L01_Init();				//���߳�ʼ��
	while (NRF24L01_Check())		//��������Ƿ�ɹ�
	{
		LED1 = !LED1;
		delay_ms(100);
	}
	NRF24L01_RX_Mode();				//��������Ϊ����ģʽ
	
	MPU_Init();						//��ʼ��MPU6050
	while (mpu_dmp_init())			//��ʼ��DMP,������Ƿ�ɹ�
	{
		LED1 = !LED1;
		delay_ms(100);
	}
	LED1 = 1;
	
	delay_ms(1000);
	
	MotorPWMInit(7199,0);			//��ʼ�����ڵ������·PWM��,10KHz
	TIM1_IntInit(4999,71);			//��ʼ����ʱ��1,����5ms�ж�
	
	while(1)
	{	
		Usart1_Dispose();	//�������ݴ���
		NRF_Receive();		//���߽�������
	}
}


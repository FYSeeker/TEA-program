#include "main.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);		//�����ж����ȼ�����2
	delay_init();					//��ʱ������ʼ��
	uart_init(9600);				//���ڳ�ʼ��Ϊ9600
	JTAG_Set(JTAG_SWD_DISABLE);		//�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);			//��SWD�ӿ�
	LedInit();						//��ʼ��LED
	KeyInit();						//��ʼ������
	Tim2EncoderInit();				//��ʼ��������1
	Tim4EncoderInit();				//��ʼ��������2
	MotorInit();					//��ʼ���������������
	MotorPwmInit();					//��ʼ�����ڵ������·PWM��,10KHz
	Tim1TimeIntInit();				//��ʼ����ʱ��5,����5ms�ж�
	
	MPU_Init();						//��ʼ��MPU6050
	while (mpu_dmp_init())			//��ʼ��DMP,������Ƿ�ɹ�
	{
		LED = !LED;
		delay_ms(10);
	}
	LED = 1;
	
	while(1)
	{
		KeyStartAndStop();		//���������͹ر�С��
		Usart1_Dispose();		//�������ݴ���
	}
}


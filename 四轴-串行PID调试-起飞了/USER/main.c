#include "delay.h"
#include "usart.h"
#include "sys.h"
#include "led.h"
#include "key.h"
#include "pwm.h"
#include "timer.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "control.h"
#include "myusart.h"
#include "spi.h"
#include "24l01.h"


int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	
	LED_Init();                	// LED��ʼ��
	KEY_Init();				   	// ������ʼ��
	delay_init();	    	   	// ��ʱ������ʼ��
	uart_init(115200);		   	// ���ڳ�ʼ��
	
	MPU_Init();					// ��ʼ��MPU6050
	while(mpu_dmp_init())		// һ��Ҫ��ʼ���ɹ�
	{LED0_Flash(666);}
	
	TIM2_Init(7199,49);			// Ƶ��200HZ
	Motor_SelfCheck();  		// ��ˢ����Լ캯�� 
	
	NRF24L01_Init();    	  	// ��ʼ��NRF24L01 
	while(NRF24L01_Check()==1)  // ���NRF24L01
	{LED0_Flash(666);}LED0=1;
	NRF24L01_RX_Mode();     	// ��������Ϊ����ģʽ 
	
	TIM1_Init(49,7199);         // 5MS��һ���жϷ��������жϷ�������control.c
	
	while(1)
	{	
//		PC_data();  			// ����λ������ŷ��������  ʱ�䣺3.62ms
		USART_Adjust();  		// �������ݴ���
		Telecontrol();  		// ң�ؿ��� 
	} 
}




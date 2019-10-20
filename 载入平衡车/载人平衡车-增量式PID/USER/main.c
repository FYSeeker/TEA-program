#include "sys.h"
#include "usart.h"		
#include "delay.h"	
#include "led.h"   
#include "key.h"   
#include "mpu6050.h" 
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h" 
#include "encoder.h" 
#include "pwm.h" 
#include "timer.h" 
#include "myusart.h"
#include "blance.h"
#include "24l01.h"

extern float roll[2];					//ŷ����


/*********************************************************************
�������ܣ����������ǳ�ʼ��
��ڲ�������
���ز�������
**********************************************************************/
void mpu_6050_init(void)
{
	u8 val=0;
	
	USE_MPU = MPU_FIRST;
	while(MPU_Init(USE_MPU))					//��ʼ��MPU6050_1
	{
		printf("MPU1_Init error\r\n");
		LED0=1;
		delay_ms(100);
		LED0=0;
		delay_ms(100);
	}
	while((mpu_dmp_init(USE_MPU)))		//������1��ʼ��
	{
		printf("mpu1_dmp_init error\t%d\r\n",val);
		LED0=1;
		delay_ms(100);
		LED0=0;
		delay_ms(100);
	}
	
	USE_MPU = MPU_SECOND;
	while(MPU_Init(USE_MPU))				//��ʼ��MPU6050_2
	{
		printf("MPU2_Init error\r\n");
		LED1=1;
		delay_ms(100);
		LED1=0;
		delay_ms(100);
	}	
	while((mpu_dmp_init(USE_MPU)))		//������2��ʼ��
	{
		printf("mpu2_dmp_init error\t%d\r\n",val);
		LED1=1;
		delay_ms(100);
		LED1=0;
		delay_ms(100);
	} 
}


/*********************************************************************
�������ܣ����߳�ʼ�����Լ�
��ڲ�������
���ز�������
**********************************************************************/
void NRF24L01_init()
{
	NRF24L01_Init();    	  		// nrf24l01 init
	
	while(NRF24L01_Check()==1)  	// nrf24L01 check
	{
		LED0_Flash(666);
	}
	LED0=1;
	
	NRF24L01_RX_Mode();     		// set to receive mode
}


/*********************************************************************
�������ܣ�һ���ͺ��˲�
��ڲ���������������
���ز�������
**********************************************************************/
void FirstOrder_Filtering(short* value)
{
	static short LastValue=0;
	
	*value = (*value) * 0.3 + LastValue * 0.7;
	LastValue = *value;
}


/*********************************************************************
�������ܣ���ȡ����������
��ڲ�������
���ز�������
**********************************************************************/
void Get_MPU6050()
{
	USE_MPU = MPU_FIRST;
	if(mpu_dmp_get_data(roll)==0)	//��ȡ��dmp����
	{							
		if(roll[0] < 0) roll[0] += 180;					
		else roll[0] = roll[0] - 180;
		
		FirstOrder_Filtering(&(Gyro[0][0]));
	}
	
	USE_MPU = MPU_SECOND;
	if(mpu_dmp_get_data(roll)==0)	//��ȡ��dmp����
	{			
//		if(roll[1] < 0) roll[1] += 180;					
//		else roll[1] = roll[1] - 180;	
		
		FirstOrder_Filtering(&(Gyro[1][0]));			
	}
	
//	printf("MPU_1: %f\t%d\t MPU_2: %f\t%d\r\n",roll[0],Gyro[0][0],roll[1],Gyro[1][0]);
}


/*********************************************************************
�������ܣ�����ƽ�⳵������
��ڲ�������
���ز�������
**********************************************************************/
int main(void)
{ 
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	delay_init();	    	 		//��ʱ������ʼ��	  
	uart_init(UAR1,9600);	 		//���ڳ�ʼ��
	LED_Init();		  				//��ʼ����LED���ӵ�Ӳ���ӿ�
	KEY_Init();						//��ʼ������
	
	mpu_6050_init();				//���������ǳ�ʼ��
//	NRF24L01_init();				// nrf24l01 init and check
		
	MOTO_DIR_Init();
	TIM2_PWM_Init(7199,0);		
		
	TIM1_Init(4999,71);  			// 5�����ж�

	while(1)
	{	
		Get_MPU6050();				//��ȡ����������
		Uart_DealDate();			//���ڴ���
		
		/*��������*/
		if(KEY_Scan(0) == KEY0_PRES)
		{
			ARMED = !ARMED;
		}	
	} 	
}



/**********
*�Ƕ�
*����ʱ��ǰ��+		��-
*����ʱ��ǰ��+		��-
*
*
*������
*����ʱ��ǰ��+		��-
*����ʱ��ǰ��+		��-
*
****/


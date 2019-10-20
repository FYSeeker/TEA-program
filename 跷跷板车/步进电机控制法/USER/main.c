#include "main.h"


/**************************************************************************************
�������ܣ�	LED����˸
��ڲ�����	LEDx��ѡ����˸�ĵ�
			time����˸ʱ��
���ز�����	��	
***************************************************************************************/
void LED_Flash(LEDx ledx,u16 time)
{
	static u16 t=0;
	
	delay_ms(1);
	t++;
	if(t>time)
	{
		t = 0;
		if(ledx==LED_0)
		{
			LED0 = !LED0;
		}
		else if(ledx==LED_1)
		{		
			LED1 = !LED1;
		}
	}
}


/**************************************************************************************
�������ܣ�	������
��ڲ�����	��
���ز�����	��	
***************************************************************************************/
int main(void)
{	
	u8 key=0;
	
	delay_init();					//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����2
	uart_init(UAR1,115200);			//���ڳ�ʼ��Ϊ115200
	JTAG_Set(JTAG_SWD_DISABLE);		//�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);			//��SWD�ӿ�
	LED_Init();						//��ʼ��LED
	KEY_Init();						//��ʼ������
	IrTube_Init();					//����Թܳ�ʼ��
	
//	NRF24L01_Init();
//	while(NRF24L01_Check())
//	{
//		LED_Flash(LED_0,50);
//	}
//	NRF24L01_RX_Mode();				//��ʼ�����ߵ�����ģʽ
	
	MPU_Init();						//��ʼ��MPU6050
	while (mpu_dmp_init())			//��ʼ��DMP,������Ƿ�ɹ�
	{
		LED_Flash(LED_0,50);
	}
	LED0 = 0;

	TIM2_PWM_init(7199,0);			//��ʱ��PWM����ʼ����10KHz
	TIM1_IntInit(499,71);			//��ʼ����ʱ��1,����50us�ж�
	
	while(1)
	{			
//		key = KEY_Scan(0);
//		if(key==KEY1_PRES)
//		{
//			Run = !Run;
//		}
		
//		Uart_DealDate();
//		LED_Flash(LED_0,500);
	}
}


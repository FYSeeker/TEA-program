#include "sys.h" 	
#include "delay.h"	
#include "led.h"


int main(void)
{				  
	Stm32_Clock_Init(9);//ϵͳʱ������
	delay_init(72);	  	//��ʱ��ʼ��
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
	while(1)
	{
		GPIO_L;
		delay_ms(300);
		GPIO_H;
		delay_ms(300);
	}	 
}

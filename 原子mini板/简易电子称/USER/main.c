#include "sys.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "lcd.h"
#include "touch.h" 
#include "timer.h"
#include "my_gui.h"
#include "adc7606.h"


extern int16_t s_volt[8];

int main(void)
{	
	u8 i;
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	delay_init();	    	   //��ʱ������ʼ��	  
	uart_init(9600);	 	   //���ڳ�ʼ��Ϊ9600
 	LED_Init();			       //LED�˿ڳ�ʼ��
	LCD_Init();	               //LCD����ʼ��
	KEY_Init();	 			   //������ʼ��
 	tp_dev.init();			   //��������ʼ�� 
	AD7606_Init();			   //AD7606ģ���ʼ��	

	Basic_Interface();        //��������
	LCD_Keyboard();           //����
	
	while(1)
	{ 
		LCD_Touch();  	//������
		LCD_Input();  	//��������
		if(i>=20)
		{
			AD7606_Scan();			//��ȡADת��������
			AD7606_Count();			//��������������
			i = 0;
		}
		LCD_Count(s_volt[0]);	//�����ѹֵ��������
		i++;
		delay_ms(5);
	}
}




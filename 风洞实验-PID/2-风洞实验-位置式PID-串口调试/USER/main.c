#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ultrasonic.h"
#include "timer.h"
#include "pid.h"


float Distance = 0;


/************************************************
	�綴���ԣ�����
************************************************/
 	
int main(void)
{	 
	u16 num,x;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	//���ڳ�ʼ��Ϊ115200
	LED_Init();			//LED�˿ڳ�ʼ��
	LCD_Init();
	TIM2_Init(60000-1, 72-1);	//TIM2�жϳ�ʼ��,Ƶ��1M��������60000Ϊ60ms
	TIM3_PWM_Init(7200-1,0);	//TIM3_PWM��ʼ��,10KHz
	Ultrasonic_Init();			//��������ʼ��
	PID_Init();  
	
	LCD_ShowString(20,50,220,16,16,"Distance: 000.00 cm");
	LCD_ShowString(20,100,220,16,16,"pwmvalue:00000");
	LCD_ShowxNum(92,120,5760,5,16,0x80);
	for(x=0;x<3;x++)
	{
		Distance = Ultrasonic_Ranging();
	}
	
	TIM5_Init(15000-1, 72-1);	//TIM5�жϳ�ʼ��������15ms
	 
	while(1) 
	{	
		Usart1_Dispose();	//���ڵ��Բ�������
		
		if(nvic_flog==1)
		{
			nvic_flog = 0;
			Distance = Ultrasonic_Ranging();
		}
		if(Distance==-1)
		{
			LCD_ShowString(100,50,56,16,16," error ");
		}
		else
		{
			num = Distance*100;
			LCD_ShowxNum(100,50,num/100,3,16,0x80);
			LCD_ShowString(124,50,8,16,16,".");	//��ʾ"."
			LCD_ShowxNum(132,50,num%100,2,16,0x80);					
		}
		
		if(x%100==0)  LED1=!LED1;
		x++;
		if(x>20000)  x=0;				   		 
		delay_ms(1);	
	} 
}

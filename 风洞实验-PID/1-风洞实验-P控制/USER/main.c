#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "lcd.h"
#include "usart.h"
#include "ultrasonic.h"
#include "timer.h"
#include "pid.h"


u8 nvic_flog = 0;

/************************************************
************************************************/
 	
int main(void)
{	 
	u8 key;
	u16 num,x,pwmvalue=2000;
	float dis,pid_value;
	
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
	LED_Init();			    //LED�˿ڳ�ʼ��
	KEY_Init();
	LCD_Init();
	TIM2_Capture_Init(60000-1, 72-1);	//TIM2�жϳ�ʼ��,Ƶ��1M��������60000Ϊ60ms
	TIM3_PWM_Init(10000-1,72-1);		//TIM3_PWM��ʼ��
	Ultrasonic_Init();		//��������ʼ��  
	
	LCD_ShowString(20,50,220,16,16,"Distance: 000.00 cm");
	 
	while(1) 
	{	
		if(x%20==0)
		{
			dis = Ultrasonic_Ranging();
			if(dis==-1)
			{
				LCD_ShowString(100,50,56,16,16," error ");
			}
			else
			{
				num = dis*100;
				LCD_ShowxNum(100,50,num/100,3,16,0x80);
				LCD_ShowString(124,50,8,16,16,".");	//��ʾ"."
				LCD_ShowxNum(132,50,num%100,2,16,0x80);
				
//				pid_value = PID_Adjust(dis);	//P����
//				if(pid_value<0) 
//				{
//					pwmvalue=0;
//				}
//				else
//				{
//					pwmvalue = pid_value;
//				}
				TIM_SetCompare1(TIM3,pwmvalue);
			}
		}
		
		/*��������*/
//		key=KEY_Scan(0);
//		if(key==KEY0_PRES && pwmvalue<=10000)
//		{
//			pwmvalue+=500;
//			key=0;
//		}
//		if(key==KEY1_PRES && pwmvalue!=0)
//		{
//			pwmvalue-=500;
//			key=0;
//		}
			
		
		if(x%300==0)  LED1=!LED1;
		x++;
		if(x>60000)  x=0;				   		 
		delay_ms(1);	

	} 
}

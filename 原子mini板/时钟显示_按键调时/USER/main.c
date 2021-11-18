
#include "led.h"
#include "delay.h"
#include "sys.h"
#include "timer.h"
#include "lcd.h"
#include "display.h"
#include "key.h"

TimeStruct Time;
u8 update_flog;

/**ʱ�ӹ�����**/
void Timer_Rule()
{
	if(Time.sec>59)
	{
		Time.min++;
		Time.sec = 0;
	}		
	if(Time.min>59)
	{
		Time.hour++;
		Time.min = 0;
	}		
	if(Time.hour>23) 
	{
		Time.hour = 0;
	}
	
	if(Time.sec<0)
	{
		Time.min--;
		Time.sec = 59;
	}
	if(Time.min<0)
	{			
		Time.hour--;
		Time.min=59;
	}
		
	if(Time.hour<0)
	{
		Time.hour=23;
	}
}



/**��������ʱ��**/
void KEY_SetTimer()
{
	u8 key, set_key;
	
	key = KEY_Scan(0);
	if(key==WKUP_PRES)		//�������ü�
	{
		set_key++;
		if(set_key>3)  set_key=0;
		
		while(set_key!=0)
		{	
			TIM_Cmd(TIM3, DISABLE);		//TIM3��ʱ��ʧ��
			
			switch(set_key)
			{
				case 1:
					Add_Icon(144,206-1);
					Subtract_Icon(144,254);
					break;
				
				case 2:
					LCD_Fill(144,206-1,144+24,206-1+24,BLACK);	//ˢ����������
					LCD_Fill(144,254,144+24,256+24,BLACK);		//ˢ����������
				
					Add_Icon(108,206-1);
					Subtract_Icon(108,254);
					break;
				
				case 3:
					LCD_Fill(108,206-1,108+24,206-1+24,BLACK);	//ˢ����������
					LCD_Fill(108,254,108+24,254+24,BLACK);		//ˢ����������	
				
					Add_Icon(72,206-1);
					Subtract_Icon(72,254);
					break;
			}
			
			key = KEY_Scan(0);
			if(key==WKUP_PRES)		//�������ü�
			{
				set_key++;
				if(set_key>3)  set_key=0;
			}
			
			else if(key==KEY0_PRES)		//��ʱ��
			{
				LCD_Fill(50,30,190,170,BLACK);	//ˢ��ʱ������
				switch(set_key)
				{
					case 1:
						Time.sec++;break;
					case 2:
						Time.min++;break;
					case 3:
						Time.hour++;break;
				}
			}
			
			else if(key==KEY1_PRES)		//��ʱ��
			{
				LCD_Fill(50,30,190,170,BLACK);	//ˢ��ʱ������
				switch(set_key)
				{
					case 1:
						Time.sec--;break;
					case 2:
						Time.min--;break;
					case 3:
						Time.hour--;break;
				}
			}
			Timer_Rule();		//ʱ�亯��
			Analog_Timer_Display();	//ģ��ʱ����ʾ	
			Display_Time(Time);		//����ʱ����ʾ
		}
		LCD_Fill(72,206-1,72+24,206-1+24,BLACK);	//ˢ����������
		LCD_Fill(72,254,72+24,254+24,BLACK);		//ˢ����������
		TIM_Cmd(TIM3, ENABLE);		//TIM3��ʱ��ʹ��
	}
}

/*********************************************************************
�������ƣ�main
�������ܣ�������
�����������
���ز�������
*********************************************************************/
int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����жϷ���2
	delay_init();
	KEY_Init();
	LED_Init();
	LCD_Init();
	TIM3_Int_Init(10000-1, 7200-1);		//TIM3�жϳ�ʼ��,0.1S�ж�һ��
	
	BACK_COLOR = BLACK;		//����Ĭ�ϱ�����ɫ
	POINT_COLOR = WHITE;	//���û�����ɫ��ɫ
	LCD_Clear(BLACK);		//����Ϊ��ɫ
	
	while(1)
	{	
		KEY_SetTimer();			//������ʱ
		if(update_flog==1)		//ʱ�����
		{
			Timer_Rule();		//ʱ�亯��
			LCD_Fill(50,30,190,170,BLACK);	//ˢ��ʱ������
			Analog_Timer_Display();		//ģ��ʱ����ʾ	
			Display_Time(Time);			//����ʱ����ʾ
			
			update_flog = 0;	//������±�־���ȴ���һ�θ���
		}
	}
}


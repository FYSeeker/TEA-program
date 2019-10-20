#include "main.h"


/******************************************************************
*��������:	System_Init
*��������:	ϵͳ��ʼ��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void System_Init(void)		//ϵͳ��ʼ��
{
	u8 i=0, num=1;
	
	delay_init();	    	 	//��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 		//���ڳ�ʼ��Ϊ 115200
	LCD_Init();			   		//��ʼ��LCD 
 	Interface();				//��ʼ������
		
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,20+i,290,BarColor);		//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
 	LED_Init();		  			//��ʼ����LED���ӵ�Ӳ���ӿ�
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,40+i,290,BarColor);		//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	KEY_Init();					//��ʼ������ 
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,60+i,290,BarColor);		//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	TPAD_Init(6);				//����������ʼ��
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,80+i,290,BarColor);		//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	IR_Tube_Init();				//����Թܳ�ʼ��
	Tim4EncoderInit();			//��ʱ��������ģʽ��ʼ��
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,100+i,290,BarColor);		//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	EXTI8_Init();				//ʹ�ܶ�ʱ������
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,120+i,290,BarColor);	//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	TIM3_PWM_Init(7199, 0);		//��ʱ��3��PWM����ʼ����10KHz
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,140+i,290,BarColor);	//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	TIM8_PWM_Init(10000, 71);	//��ʱ��8��PWM����ʼ����100Hz 10000
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,160+i,290,BarColor);	//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	OV7670_init();				//OV7670��ʼ�����
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,180+i,290,BarColor);	//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	NRF24L01_init();			//����ģ���ʼ�����
	
	for(i=1;i<21;i++,num++)
	{
		LCD_Fill(20,270,200+i,290,BarColor);	//��ʾ������
		LCD_ShowNum(104,300,num/2,3,16);		//��ʾ�ٷֱ�
		delay_ms(10);
	}		
	
	delay_ms(500);
	LCD_Clear(BLACK);
}


/******************************************************************
*��������:	Interface
*��������:	��ʼ������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Interface()
{
	POINT_COLOR=RED;			//��������Ϊ��ɫ 
	LCD_ShowString(0,50,240,24,24,(u8*)"Goods Sorting System");
	LCD_ShowString(30,90,200,16,16,(u8*)"SYSTEM Init...");	
	delay_ms(500);	
	LCD_Fill(19,269,221,291,LIGHTBLUE);	//��ʾ������
	LCD_ShowString(128,300,8,16,16,(u8*)"%");
}


/******************************************************************
*��������:	OV7670_init
*��������:	��ʼ������ͷ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void OV7670_init()
{
	LCD_ShowString(30,110,200,16,16,(u8*)"OV7670 Init...");	
	
	while(OV7670_Init())	//��ʼ��OV7670
	{
		LCD_ShowString(30,130,200,16,16,(u8*)"OV7670 Error!!!");
		delay_ms(200);
	    LCD_Fill(30,110,230,130,WHITE);
		delay_ms(200);
	}
	
 	LCD_ShowString(30,130,200,16,16,(u8*)"OV7670 Init OK");
	delay_ms(300);
	OV7670_Config();		//OV7670��ʼ����
}


/******************************************************************
*��������:	NRF24L01_init
*��������:	��ʼ������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void NRF24L01_init()
{
	LCD_ShowString(30,150,200,16,16,(u8*)"NRF24L01 Init...");	
	
	NRF24L01_Init();		//��ʼ��NRF24L01
	
	while(NRF24L01_Check())	//�����Լ�
	{
		LCD_ShowString(30,170,200,16,16,(u8*)"NRF24L01 Error!!!");
		delay_ms(200);
	    LCD_Fill(30,150,230,170,WHITE);
		delay_ms(200);
	}
 	LCD_ShowString(30,170,200,16,16,(u8*)"NRF24L01 Init OK");
	delay_ms(500);
}


/******************************************************************
*��������:	Sys_Start
*��������:	ϵͳ������������
*��������:	��
*�� �� ֵ:	0:û�������ź�
			1:�������ź�
*******************************************************************/
u8 Sys_Start()
{
	u8 Nrf_TX_buf[6]={"start"};	//��������
	
	if(TPAD_Scan(0) == 1)			//��⵽���� 
	{
		NRF24L01_TX_Mode();		//��������Ϊ����ģʽ
		
		if(MAX_TX == NRF24L01_TxPacket(Nrf_TX_buf))	//���߷������������С��
		{			
//			NRF24L01_RX_Mode();	//��������Ϊ����ģʽ
			LED0 = 0;
		}
		return 1;
	}
	else 
	{	
		return 0;
	}
}


/******************************************************************
*��������:	Led_Test
*��������:	LED������˸
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Led_Test(u16 time)
{
	static u16 i = 0;
	
	i++;
	if(i>59999) i=0;
	
	if(i%time==0)			//DS0��˸.
	{
		LED0=!LED0;
	}
	delay_us(10);
}



u8 Conveyor_Run=0;	//���ʹ����б��

/******************************************************************
*��������:	main
*��������:	������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
int main(void)
{	
	u8 System_Run=0;	//ϵͳ���б�־
	
	System_Init();		//ϵͳ��ʼ��
	
 	while(1) 
	{	
		Camera_refresh();			//����LCD��ʾ	
		
		if(System_Run==0)			//ϵͳû������
		{
			if(Sys_Start())			//ϵͳ����
			{	
				Conveyor_Advance();	//�������ʹ�
				System_Run = 1;		//���ϵͳ����
			}
					
		}
	
		if((TIM4->CNT > 3000)&&(TIM4->CNT<4000))
		{
			TIM4->CNT = 0;		//������������0
			Conveyor_Stop();
			System_Run = 0;
			LED0 = 1;
		}
		
		if(IR == 0)		//��⵽ľ��
		{	
			delay_ms(5);
			if(IR == 0)
			{
				LED1 = 1;
				
				if(System_Run)				//ϵͳ����
				{
					if(Conveyor_Run)		//���ʹ�����
					{			
						Conveyor_Stop();	//ֹͣ���ʹ�
						TIM4->CNT = 0;		//������������0
						Discern_Flag = 1;	//�����ɫʶ��	
					}
				}
			}
		}
		else
		{
			LED1 = 0;
		}
		
		if(over)		//ʶ�����
		{
			Result_Handler(COLOR);
			over = 0;
			
			if(IR==1)
			{
				Conveyor_Advance();	//�������ʹ�
			}
			else
			{
				Discern_Flag = 1;	//�����ɫʶ��				
			}
		}
			
//		Led_Test(30);	
	}	   
}



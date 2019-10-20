#include "main.h"


u8 LOCK=0;	//����������

/*
����BUF
TXbuf[0]: 0��������1������ 
TXbuf[1]: 0���޶�����1�������ţ�2��������
TXbuf[2]: 0���޶�����1��ǰ��2����3����4����
*/
u8 TXbuf[3]={0};

/**************************************************************************************
�������ܣ�	LED����˸
��ڲ�����	LEDx��ѡ����˸�ĵ�
			xms����˸ʱ��,ms
���ز�����	��	
***************************************************************************************/
void LED_Flash(LEDx ledx,u16 xms)
{
	static u16 t_us=0,t_ms=0;
	
	delay_10us(1);
	t_us++;
	if(t_us>90)
	{
		t_us=0;
		t_ms++;
		if(t_ms>xms)
		{
			t_ms = 0;
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
}

/*************************************************************************
�������ܣ�	������
��ڲ�����	��
���ز�����	��
**************************************************************************/
void main()
{
	u16 key=0;

//	USART_Init(9600);		//��ʼ�����ڣ�������9600
	Nrf24l01Init();			//���߳�ʼ��
	while(CheckNrf24l01())	//�����Լ�
	{
		LED1 = !LED1;
		delay_ms(100);
	}
	LED1 = 1;
	Nrf24l01SetTXMode();	//��������Ϊ����ģʽ

	TIM0Int_Init(60000);	//��ʱ��0�жϳ�ʼ����60ms
	   
	while(1)
	{
		if(KEY1==0)
		{
			delay_ms(10);
			if(KEY1==0)
			{
				LOCK = !LOCK; 	//����������
				while(!KEY1);	//���ּ��
			}
		}

		if(LOCK)		//����
		{		
			LED1 = 0;

			key = KEY_Scan(1);
			
			if(key!=0)
			{	
				if(key==KEY0_VALUE)
				{
					TXbuf[1]=1;	
				}
				else if(key==KEY2_VALUE)
				{
					
					TXbuf[1]=2;	
				}
				else
				{
				   switch(key)
				   {
				   		case KEY3_VALUE:	//ǰ
							TXbuf[2]=1;
							break; 
				   		case KEY4_VALUE:	//��
							TXbuf[2]=3;
							break;
				   		case KEY5_VALUE:	//�� 
							TXbuf[2]=2;
							break;
				   		case KEY6_VALUE:	//��
							TXbuf[2]=4;
							break;
						default:
							TXbuf[2]=0;
				   }
				}
			}
			else
			{	
				LED1 = 1;			 
				TXbuf[1]=0;
				TXbuf[2]=0;
			}
		}
		else
		{
			LED1 = 1;
		}
	}	
}
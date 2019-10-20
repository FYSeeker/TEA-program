#include "usart.h"

void USART_Init()
{
	PCON &= 0x7F;	//�����ʲ�����
	SCON = 0x50;	//8λ����,�ɱ䲨����
	TMOD &= 0x0F;	//�����ʱ��1ģʽλ
	TMOD |= 0x20;	//�趨��ʱ��1Ϊ8λ�Զ���װ��ʽ
	TL1 = 0xFD;		//�趨��ʱ��ֵ,������9600
	TH1 = 0xFD;		//�趨��ʱ����װֵ
	ET1 = 0;		//��ֹ��ʱ��1�ж�
	TR1 = 1;		//������ʱ��1	
}


void USART_Send()
{
	static int n=0;
	unsigned char i=0;
	unsigned char key = 0;

	code unsigned char buf_1[]="s1:0\r\n";
	code unsigned char buf_2[]="s2:0\r\n";
	code unsigned char buf_3[]="s3:0\r\n";
	code unsigned char buf_4[]="s4:0\r\n";
	code unsigned char buf_5[]="s5:0\r\n";
	code unsigned char buf_6[]="s6:0\r\n";
	code unsigned char buf_7[]="st:0\r\n";

	key = KEY_Scan();

	if(key>6)
	{	
		switch(key)
		{
			case 10:
				i=0;
				while(buf_5[i]!='\0')
				{
					SBUF = buf_5[i++];
					while(!TI);
					TI = 0;
				}
				break;
	
			case 7:
				i=0;
				while(buf_6[i]!='\0')
				{
					SBUF = buf_6[i++];
					while(!TI);
					TI = 0;
				}
				break;
		}
		while(P3!=0XFF);	//��߰��£����ּ��	
	}
	else
	{
		n++;
		if(n>1999)  n=0;
		delay_ms(1);
	
		if(n%50==0)
		{
			switch(key)
			{
		
				case 6:
					i=0;
					while(buf_1[i]!='\0')
					{
						SBUF = buf_1[i++];
						while(!TI);
						TI = 0;
					}
					break;
		
				case 3:
					i=0;
					while(buf_2[i]!='\0')
					{
						SBUF = buf_2[i++];
						while(!TI);
						TI = 0;
					}
					break;
		
				case 4:
					i=0;
					while(buf_3[i]!='\0')
					{		 
						SBUF = buf_3[i++];
						while(!TI);
						TI = 0;
					}
					break;
		
				case 5:
					i=0;
					while(buf_4[i]!='\0')
					{
						SBUF = buf_4[i++];
						while(!TI);
						TI = 0;
					}
					break;
		
				case 0:
					i=0;
					while(buf_7[i]!='\0')
					{
						SBUF = buf_7[i++];
						while(!TI);
						TI = 0;
					}
					break;
			}
		}
	}
}
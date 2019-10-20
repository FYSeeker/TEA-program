#include "wkup.h"
#include "delay.h"
#include "key.h"


	 
void Sys_Sta(void)
{  
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);	//ʹ��PWR����ʱ��
	PWR_WakeUpPinCmd(ENABLE);  //ʹ�ܻ��ѹܽŹ���
	PWR_EnterSTANDBYMode();	  //���������STANDBY��ģʽ 		 
}
//ϵͳ�������ģʽ
void Enter_Standby(void)
{			 
	RCC_APB2PeriphResetCmd(0X01FC,DISABLE);	//��λ����IO��
	Sys_Sta();
}

void WK_UPStandby(void)
{
	if(WK_UP)
	{	
		delay_ms(10);
		if(WK_UP)
		{
			while(WK_UP);
			delay_ms(500);
			Enter_Standby();
		}
	}
}
















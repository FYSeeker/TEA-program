#include "key.h"
#include "delay.h"

/******************************************************************
*��������:	KEY_Init
*��������:	��ʼ������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void KEY_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);	//ʹ��PB�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_7;	//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIOB 
}


u8 KEY_Scan()
{	 
	static u8 key_up=1;//�������ɿ���־
	
	if(key_up && (READ_KEY0==0||READ_KEY1==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		
		if(READ_KEY0==0)		return KEY0_PRES;
		else if(READ_KEY1==0)	return KEY1_PRES;
	}
	else if(READ_KEY0==1&&READ_KEY1==1)
	{
		key_up=1;
	} 	    
 	return 0;// �ް�������	
}

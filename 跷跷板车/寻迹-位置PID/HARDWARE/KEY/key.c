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
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_GPIOC, ENABLE);	//ʹ��IO�˿�ʱ��
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//��������
	GPIO_Init(GPIOC, &GPIO_InitStructure);					//�����趨������ʼ��GPIO 
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;			//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;				//�˿�����
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;			//��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);					//�����趨������ʼ��GPIO
}


/******************************************************************
*��������:	KEY_Scan
*��������:	����������
*��������:	mode:0,��֧��������;1,֧��������;
*�� �� ֵ:	���ذ���ֵ
			0��û���κΰ�������
			KEY0_PRES��KEY0����
			KEY1_PRES��KEY1����
			WKUP_PRES��WK_UP����
*ע��˺�������Ӧ���ȼ�,KEY0>KEY1>WK_UP!!
*******************************************************************/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;		//�������ɿ���־
	
	if(mode)key_up=1;		//֧������		  
	if(key_up&&(KEY0==0||KEY1==0||WK_UP==1))
	{
		delay_ms(1);		//ȥ���� 
		key_up=0;
		if(KEY0==0)	return KEY0_PRES;
		else if(KEY1==0) return KEY1_PRES;
		else if(WK_UP==1) return WKUP_PRES; 
	}
	else if(KEY0==1&&KEY1==1&&WK_UP==0) key_up=1; 	     
	return 0;// �ް�������
}


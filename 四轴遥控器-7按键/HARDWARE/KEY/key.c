#include "key.h"


/*************************************************************************
�������ܣ�	����ɨ�躯��
��ڲ�����	mode:0,��֧��������;1,֧��������;
���ز�����	���ذ���ֵ
����˵����	��������Ӧ���ȼ�,KEY0>KEY1>KEY2>KEY3������
**************************************************************************/
u8 KEY_Scan(u8 mode)
{	 
	static u8 key_up=1;//�������ɿ���־

	if(mode) key_up=1;  //֧������		  
	if(key_up&&(KEY0==0||KEY1==0||KEY2==0||KEY3==0||KEY4==0||KEY5==0||KEY6==0))
	{
		delay_ms(10);//ȥ���� 
		key_up=0;
		if(KEY0==0)		return KEY0_VALUE;
		else if(KEY1==0)return KEY1_VALUE;
		else if(KEY2==0)return KEY2_VALUE;
		else if(KEY3==0)return KEY3_VALUE; 
		else if(KEY4==0)return KEY4_VALUE;
		else if(KEY5==0)return KEY5_VALUE;
		else if(KEY6==0)return KEY6_VALUE;
	}
	else if(KEY0==1&&KEY1==1&&KEY2==1&&KEY3==1&&KEY4==1&&KEY5==1&&KEY6==1)
	{
		key_up=1; 
	}	    
 	return 0;// �ް�������
}

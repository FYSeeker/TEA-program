#include "key.h"


unsigned char KEY_Scan()
{
	unsigned char key_flag = 0;

	if(P2!=0xff)		//�ұ��а�������
	{
		delay_ms(5);
		if(P2!=0xff)	//�ұ��а�������
		{
			switch (P2)
			{
				case 0xfe:
					key_flag = 1;
					break;
				case 0xfd:
					key_flag = 2;
					break;
				case 0xfb:
					key_flag = 3;	//����
					break;
				case 0xf7:
					key_flag = 4;	//����
					break;
				case 0xef:
					key_flag = 5;	//����
					break;
				case 0xdf:
					key_flag = 6;	//����
					break;
				default:key_flag = 0;	
			}
		}
	}

	if(P3!=0xff)		//����а�������
	{
		delay_ms(5);	//����а�������
		if(P3!=0xff)
		{
			switch (P3)
			{
				case 0x7f:
					key_flag = 7;	//����
					break;
				case 0xbf:
					key_flag = 8;	//����
					break;
				case 0xdf:
					key_flag = 9; 	//����
					break;
				case 0xef:
					key_flag = 10;	//����
					break;
				default:key_flag = 0;
			}
		}
	}

	return key_flag;
}
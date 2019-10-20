#ifndef KEY_H_
#define KEY_H_

#include "sys.h"

#define KEY0 PBin(9)
#define KEY1 PBin(7)

#define READ_KEY0	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_9)//��ȡ����0
#define READ_KEY1	GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_7)//��ȡ����1 

#define KEY0_PRES 	1	//KEY0����
#define KEY1_PRES	2	//KEY1����

/******************************************************************
*��������:	KEY_Init
*��������:	��ʼ������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void KEY_Init(void);

u8 KEY_Scan(void);

#endif

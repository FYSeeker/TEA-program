#ifndef LED_H_
#define LED_H_

#include "sys.h"

#define LED0 PBout(8)	//PB8
#define LED1 PBout(9)	//PB9

/******************************************************************
*��������:	LED_Init
*��������:	��ʼ��LED
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void LED_Init(void);

#endif

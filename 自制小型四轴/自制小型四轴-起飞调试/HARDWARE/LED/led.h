#ifndef LED_H_
#define LED_H_

#include "sys.h"

#define LED0 PBout(10)	//PB10
#define LED1 PBout(1)	//PB1


/******************************************************************
*��������:	LED_Init
*��������:	��ʼ��LED
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void LED_Init(void);

#endif

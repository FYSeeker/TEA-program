#ifndef _KEY_H
#define _KEY_H
#include "sys.h"



//����ķ�ʽ��ͨ��ֱ�Ӳ���HAL�⺯����ʽ��ȡIO
#define KEY0        HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_10)	//KEY0����
#define KEY1        HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_9)  //KEY1����
#define WK_UP       HAL_GPIO_ReadPin(GPIOI,GPIO_PIN_11)	//WKUP����

#define KEY0_PRES 	1
#define KEY1_PRES	2
#define WKUP_PRES   3

void KEY_Init(void);
u8 KEY_Scan(u8 mode);
#endif

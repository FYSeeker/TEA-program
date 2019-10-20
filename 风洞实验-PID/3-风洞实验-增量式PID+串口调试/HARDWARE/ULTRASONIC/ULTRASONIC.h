#ifndef _ultrasonic_h_
#define _ultrasonic_h_

#include "sys.h"
#include "delay.h"
#include "lcd.h"

#define TRIG PAout(4)	//��������,����10us�ߵ�ƽ����
#define ECHO PAin(1)	//�������ţ���øߵ�ƽ����ʱ��

#define RCC_Ultrasonic_TRIG	RCC_APB2Periph_GPIOA
#define RCC_Ultrasonic_ECHO RCC_APB2Periph_GPIOA

#define GPIO_Ultrasonic_TRIG GPIOA
#define GPIO_Ultrasonic_ECHO GPIOA

#define PIN_Ultrasonic_TRIG GPIO_Pin_4
#define PIN_Ultrasonic_ECHO GPIO_Pin_1

void Ultrasonic_Init(void);	//��������ʼ��

float Ultrasonic_Ranging(void);

#endif

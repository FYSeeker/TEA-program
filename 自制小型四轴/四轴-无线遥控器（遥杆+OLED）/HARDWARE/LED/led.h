#ifndef __LED_H
#define __LED_H	 
#include "sys.h"



#define LED0 PBout(12)	

void LED_Init(void);//��ʼ��
void LED0_Flash(u16 time);//��˸����
		 				    
#endif

#ifndef __LED_H
#define __LED_H	 
#include "sys.h"


#define LED0 PBout(5)// PB5
#define LED1 PEout(5)// PE5	

#define Bluetooth_P PBin(0)	//�����˿�


/*LED�˿ڳ�ʼ��*/
void LED_Init(void);//��ʼ��

/*�����˿ڳ�ʼ��*/
void BluetoothIO_init(void);
		 				    
#endif

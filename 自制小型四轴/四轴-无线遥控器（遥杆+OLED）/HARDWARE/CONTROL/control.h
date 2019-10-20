#ifndef __CONTROL_H
#define __CONTROL_H
#include "sys.h"

extern float ADCX[4]; 
extern u16 THROTTLR;
extern u8  ARMED;

extern u8 Before_after;  	// ǰ���־  
extern u8 Right_left;  		// ���ұ�־  
extern u8 Rotation;  		// ��ת��־ 


void ADC_Gather(void);
void KEY_Control(void);
void NRF_SendData(void);

int myabs(int a);  // ����ֵ����


#endif

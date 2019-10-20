#ifndef _display_h_
#define _display_h_

#include "sys.h"

extern u8 Discern_Flag;	//��ɫʶ���־
extern u8 over;			//�ж�������ʾ
extern u8 COLOR;		//�ж����

#define Sampling_Number 15	//��������(>=7)
#define Verdict_Nunber 41	//�жϴ���

/*��ȡ����ͷ��һ�����ص�*/
#define READ_FIFO_PIXEL(RGB565)		OV7670_RCK_L;\
									color = GPIOC->IDR & 0XFF;\
									OV7670_RCK_H;\
									color <<= 8;\
									OV7670_RCK_L;\
									color |= GPIOC->IDR & 0XFF;\
									OV7670_RCK_H;\

void Camera_refresh(void);
void Data_Sort1(u16* Color);
u16 Data_Sort2(u16* array,u16 number);
u32 Average_Filter(u16* Color);
u16 Maximum(u16* array);
void Set_OV7670(void);				//��������OV7670

#endif

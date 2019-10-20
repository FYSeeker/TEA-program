#ifndef _handle_h_
#define _handle_h_

#include "sys.h"
#include "timer.h"
#include "delay.h"
#include "usart.h"
#include "main.h"

#define Variate 200

void Sinistrogyration(void);	//�������
void Dextroversion(void);		//�������
void Steering_Reset(void);		//�����λ
void Conveyor_Advance(void);	//���ʹ���ǰ
void Conveyor_Recede(void);		//���ʹ����
void Conveyor_Stop(void);		//ֹͣ���ʹ�
void Push_1(void);				//����1��
void Push_2(void);				//����2��
void Push_3(void);				//����3��

#endif

#ifndef _timer_h_
#define	_timer_h_

#include "sys.h"

typedef struct
{
	int8_t time;
	int8_t hour;
	int8_t min;
	int8_t sec;
}TimeStruct;		//ʱ,��,��,�ṹ��

void TIM3_Int_Init(u16 arr, u16 psc);

#endif

#ifndef _sys_h_
#define _sys_h_

#include<reg52.h>
#include "stdio.h"

/*�û�����*/
sbit LED0 = P0^0; 
sbit LED1 = P0^1;

/*�������Ͷ���*/
typedef unsigned char		u8;
typedef unsigned int		u16;
typedef xdata unsigned char xu8;
typedef xdata unsigned int	xu16;

typedef signed char			s8;
typedef signed int			s16;

/********************ϵͳʱ������********************/	  
/*ϵͳ����ʱ��*/
#define SysClock 11059200UL	 
//#define SysClock 12000000UL
/*ϵͳ��Ƶϵ��*/
#define SysPsc 12


/*************************************************************************
�������ܣ�	��ʱ��������λ��us��
��ڲ�����	t����ʱʱ�䣨t*10us��
���ز�����	��
**************************************************************************/
void delay_10us(u16 t);

/*************************************************************************
�������ܣ�	��ʱ��������λ��ms��
��ڲ�����	t����ʱʱ�䣨t*1ms��
���ز�����	��
**************************************************************************/
void delay_ms(u16 ms);


#endif
#ifndef __MY_GUI_H
#define __MY_GUI_H

#include "sys.h"
#include "delay.h"
#include "led.h"
#include "key.h" 
#include "lcd.h" 
#include "touch.h"
#include "usart.h"



void Basic_Interface(void);  //��������
void LCD_Keyboard(void);     //����
void LCD_Touch(void);        //������
void LCD_Trends(void);       //������̬��ʾ
void LCD_Input(void);        //������������
void LCD_Count(s16 date);  //�����ѹֵ��������



#endif



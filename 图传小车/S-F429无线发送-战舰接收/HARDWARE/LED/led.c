#include "led.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEK STM32F429������
//LED��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//��������:2015/11/23
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2014-2024
//All rights reserved									  
////////////////////////////////////////////////////////////////////////////////// 	

//��ʼ��PB1Ϊ���.��ʹ��ʱ��	    
//LED IO��ʼ��
void LED_Init(void)
{
    GPIO_InitTypeDef GPIO_Initure;
    __HAL_RCC_GPIOI_CLK_ENABLE();           //����GPIOIʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_5|GPIO_PIN_6; //PI5,6
    GPIO_Initure.Mode=GPIO_MODE_OUTPUT_PP;  //�������
    GPIO_Initure.Pull=GPIO_PULLUP;          //����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;     //����
    HAL_GPIO_Init(GPIOI,&GPIO_Initure);
	
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_SET);	//Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,GPIO_PIN_SET);	//Ĭ�ϳ�ʼ�������
}

#include "led.h"


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
	
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_5,GPIO_PIN_SET);	//PI5��1��Ĭ�ϳ�ʼ�������
    HAL_GPIO_WritePin(GPIOI,GPIO_PIN_6,GPIO_PIN_SET);	//PI6��1��Ĭ�ϳ�ʼ�������
}

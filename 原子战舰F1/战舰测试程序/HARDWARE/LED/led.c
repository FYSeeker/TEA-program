#include "led.h"


		    
//LED IO��ʼ��
void LED_Init(void)
{	   	 
	RCC->APB2ENR|=1<<2;    //ʹ��PORTAʱ��
	RCC->APB2ENR|=1<<3;    //ʹ��PORTBʱ��	   	 
	RCC->APB2ENR|=1<<4;    //ʹ��PORTCʱ��
	RCC->APB2ENR|=1<<5;    //ʹ��PORTDʱ��	   	 
	RCC->APB2ENR|=1<<6;    //ʹ��PORTEʱ��
	RCC->APB2ENR|=1<<7;    //ʹ��PORTFʱ��	   	 
	RCC->APB2ENR|=1<<8;    //ʹ��PORTGʱ��	
	   	 
	GPIOA->CRL = 0X33333333;//PA �������
	GPIOA->CRH = 0X33333333;//PA �������  
	GPIOB->CRL = 0X33333333;//PB ������� 
	GPIOB->CRH = 0X33333333;//PB ������� 
	GPIOC->CRL = 0X33333333;//PC �������
	GPIOC->CRH = 0X33333333;//PC �������  
	GPIOD->CRL = 0X33333333;//PD �������  
	GPIOD->CRH = 0X33333333;//PD �������  
	GPIOE->CRL = 0X33333333;//PE �������  
	GPIOE->CRH = 0X33333333;//PE �������  
	GPIOF->CRL = 0X33333333;//PF �������  
	GPIOF->CRH = 0X33333333;//PF �������  
	GPIOG->CRL = 0X33333333;//PG ������� 
	GPIOG->CRH = 0X33333333;//PG ������� 
}







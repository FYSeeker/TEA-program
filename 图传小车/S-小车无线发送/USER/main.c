#include "main.h"



/**************************************************************************************
�������ܣ�	LED����˸
��ڲ�����	LEDx��ѡ����˸�ĵ�
			time����˸ʱ��
���ز�����	��	
***************************************************************************************/
void LED_Flash(LEDx ledx,u16 time)
{
	static u16 t_1,t=0;
	
	delay_us(1);
	t_1++;
	if(t_1>499)
	{
		t_1 = 0;
		t++;
		if(t>time)
		{
			t = 0;
			if(ledx==LED_0)
			{
				LED0 = !LED0;
			}
			else if(ledx==LED_1)
			{		
				LED1 = !LED1;
			}
		}
	}
}


/**************************************************************************************
�������ܣ�	������
��ڲ�����	��
���ز�����	��
***************************************************************************************/
int main(void)
{	
	u8 key=0;
	
	delay_init();					//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����2
	uart_init(UAR1,115200);			//���ڳ�ʼ��Ϊ115200
	MyUSART3_init(115200);			//���ڳ�ʼ��Ϊ115200
	JTAG_Set(JTAG_SWD_DISABLE);		//�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);			//��SWD�ӿ�
	LED_Init();						//��ʼ��LED
	KEY_Init();						//��ʼ������
//	mallco_dev.init(SRAMIN);		//��ʼ���ڴ��
	
	NRF24L01_Init();
	while(NRF24L01_Check())
	{
		LED_Flash(LED_1,20);
	}
	NRF24L01_TX_Mode();				//��ʼ�����ߵ�����ģʽ
	
	while(OV7670_Init())			//��ʼ��OV7670
	{
		LED_Flash(LED_1,20);
	}
	OV7670_Window_Set(10,174,240,320);	//���ô���								  
	EXTI3_Init();						//ʹ�ܶ�ʱ������

	MYDMA_Config(DMA1_Channel3,(u32)&USART3->DR,(u32)&USART3_flag,1);	//����2����DMA
	
	TIM8_PWM_init(7199,0);			//��ʱ��PWM����ʼ����10KHz
	TIM1_IntInit(49999,71);			//��ʱ���жϳ�ʼ����50ms
	
	OV7670_CS=0;
	
	printf("Init OK!\r\n");
	
	while(1)
	{		
		Image_DataHandle();		//ͼ�����ݴ���
	}
}


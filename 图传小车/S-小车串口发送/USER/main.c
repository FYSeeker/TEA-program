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
	delay_init();					//��ʱ������ʼ��
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	//�����ж����ȼ�����2
	uart_init(UAR1,115200);			//���ڳ�ʼ��Ϊ115200
	MyUSART3_init(1382400);			//���ڳ�ʼ��Ϊ1382400
	JTAG_Set(JTAG_SWD_DISABLE);		//�ر�JTAG�ӿ�
	JTAG_Set(SWD_ENABLE);			//��SWD�ӿ�
	LED_Init();						//��ʼ��LED
	KEY_Init();						//��ʼ������
//	BluetoothIO_init();				//�����˿ڳ�ʼ��
	mallco_dev.init(SRAMIN);		//��ʼ���ڴ��
	TX_buf = mymalloc(SRAMIN,buf_size);
	if(TX_buf==NULL)
	{
		printf("TX buf Error!\r\n");
		while(1)
		{
			LED_Flash(LED_1,20);
		}
	}
	else printf("TX buf OK!\r\n");
	
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
	OV7670_Window_Set(10,174,OvLcdWide,OvLcdHigh);	//���ô���								  
	EXTI3_Init();						//ʹ�ܶ�ʱ������

	DMA1_Channel3_Config((u32)&USART3->DR,(u32)&USART3_flag,1);	//����3����DMA
	USART_DMACmd(USART3,USART_DMAReq_Rx,ENABLE);	//ʹ�ܴ���3����DMA
	
	DMA1_Channel2_Config((u32)&USART3->DR,(u32)TX_buf,buf_size);//����3����DMA	
	USART_DMACmd(USART3,USART_DMAReq_Tx,ENABLE);
	
//	TIM3_Int_Init(9999,7199);		//֡��ͳ��
	TIM8_PWM_init(7199,0);			//��ʱ��PWM����ʼ����10KHz
	TIM1_IntInit(29999,71);			//��ʱ���жϳ�ʼ����30ms
	
//	while(Bluetooth_P)				//�ȴ���������
//	{
//		delay_ms(1);
//	}
//	delay_ms(200);
	OV7670_CS=0;	
	printf("Init OK!\r\n");
	
	while(1)
	{		
		Image_DataHandle();		//ͼ�����ݴ���
	}
}


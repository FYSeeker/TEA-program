#include "ADC7606.h"
#include "lcd.h"

int16_t s_adc_now[8]={0};
int16_t s_volt[8]={0};
int16_t s_dat[8]={0};
/*********************************************************************************************************
*	�� �� ��: AD7606_ConfigGPIO
*	����˵��: ����GPIO�� ������ SCK  MOSI  MISO �����SPI���ߡ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void  AD7606_ConfigGPIO( void )
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);		/* PB3Ĭ��ΪJTDO���ܡ���PB3�ڸ���Ϊ��ͨIO�� */
	
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);//�ر�jtag , ����swd
	
	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_CS | RCC_RANGE | RCC_BUSY | RCC_RESET | RCC_CONVST, ENABLE);	
	
	/* ���ü����������IO */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */

	GPIO_InitStructure.GPIO_Pin = PIN_RESET;
	GPIO_Init(PORT_RESET, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_CONVST;
	GPIO_Init(PORT_CONVST, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_RANGE;
	GPIO_Init(PORT_RANGE, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PIN_CS;
	GPIO_Init(PORT_CS, &GPIO_InitStructure);

	/* ����GPIOΪ��������ģʽ(ʵ����CPU��λ���������״̬) */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO ����Ϊ�������� */

	GPIO_InitStructure.GPIO_Pin = PIN_BUSY;
	GPIO_Init(PORT_BUSY, &GPIO_InitStructure);
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_InitSPIBus
*	����˵��: ����SPI���ߡ� ֻ���� SCK�� MOSI�� MISO���ߵ����á�������ƬѡCS��Ҳ����������оƬ���е�INT��BUSY��
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_InitSPIBus(void)
{

	GPIO_InitTypeDef  GPIO_InitStructure;

	/* ��GPIOʱ�� */
	RCC_APB2PeriphClockCmd(RCC_SCK | RCC_MOSI | RCC_MISO, ENABLE);	

	/* ����SPI����SCK��MISO �� MOSIΪ��������ģʽ */
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	/* �������ģʽ */
	GPIO_InitStructure.GPIO_Pin = PIN_SCK;
	GPIO_Init(PORT_SCK, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PIN_MOSI;
	GPIO_Init(PORT_MOSI, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;		/* MISO ����Ϊ�������� */
	GPIO_InitStructure.GPIO_Pin = PIN_MISO;
	GPIO_Init(PORT_MISO, &GPIO_InitStructure);


}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_SetInputRange
*	����˵��: ����AD7606ģ���ź��������̡�
*	��    ��: _ucRange : 0 ��ʾ����5V   1��ʾ����10V
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_SetInputRange(uint8_t _ucRange)
{
	if (_ucRange == 0)
	{
		AD_RANGE_5V();	/* ����Ϊ����5V */
	}
	else
	{
		AD_RANGE_10V();	/* ����Ϊ����10V */
	}
}


/*
*********************************************************************************************************
*	�� �� ��: AD7606_RESET
*	����˵��: Ӳ����λ
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Reset(void)
{
	/* CS  SCLK���� */
#ifdef SOFT_SPI  
		AD_CS_1();
		//SCK_1();
#endif

 	/* AD7606�Ǹߵ�ƽ��λ��Ҫ����С����50ns */
	AD_RESET_LOW();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_HIGH();
	AD_RESET_LOW();

}


/*
*********************************************************************************************************
*	�� �� ��: bsp_InitAD7606
*	����˵��: ��ʼ��AD7606 SPI����
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spi_InitAD7606(void)
{	
	AD7606_SetInputRange(0);	/* ���ò������� */

	/* ���ù�����ģʽ */
	//ad7606_SetOS(0);
		
	AD_CONVST_HIGH();			/* CONVST������Ϊ�ߵ�ƽ */
	
	AD7606_Reset();				/* Ӳ����λ��AD7606 */
}
/*
*********************************************************************************************************
*	�� �� ��: bsp_spiRead1
*	����˵��: ��SPI���߽���8��bit���ݡ�  SCK�����زɼ�����, SCK����ʱΪ�ߵ�ƽ
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
uint8_t bsp_spiRead1(void)
{
	uint8_t i;
	uint8_t read = 0;
	
	for (i = 0; i < 8; i++)
	{
		SCK_0();//ʱ������
		bsp_spiDelay();//��ʱ
		read = read << 1;
		if (MISO_IS_HIGH())//���MISO=1
		{
			read|=0x01;
		}
		SCK_1();//ʱ������
		bsp_spiDelay();//��ʱ
	}
	return read;
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_scan
*	����˵��: ɨ����ñ����������ڶ�ȡADת��������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Scan(void) 		/* �˺������밴��ʱ���д */
{
	uint8_t i;			
	
	AD7606_StartConv();	/* ����ʼ�ź� */
	
	/* BUSY = 0 ʱ.ad7606���ڿ���״̬adת������ */	
	while (!BUSY_IS_LOW());	   
	
	AD_CS_0(); /* SPIƬѡ = 0 */

	for (i = 0; i < CH_NUM; i++)
	{
		s_adc_now[i] = bsp_spiRead1();
		
		s_adc_now[i] = s_adc_now[i] * 256 + bsp_spiRead1(); /* ������ */	
	}

	AD_CS_1(); /* SPIƬѡ = 1 */
}	  


/*
*********************************************************************************************************
*	�� �� ��: ad7606_StartConv
*	����˵��: ����AD7606��ADCת��
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_StartConv(void)
{
	/* �����ؿ�ʼת�����͵�ƽ����ʱ������25ns  */
	AD_CONVST_LOW();
	AD_CONVST_LOW();
	AD_CONVST_LOW();	/* ����ִ��2�Σ��͵�ƽԼ50ns */
	
	AD_CONVST_HIGH();
}

/*
*********************************************************************************************************
*	�� �� ��: bsp_SpiDelay
*	����˵��: ʱ���ӳ�
*	��    ��: ��
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void bsp_spiDelay(void)
{
	uint32_t i;

	for (i = 0; i < 2; i++);
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_ReadAdc
*	����˵��: �����ж�ȡһ·ADCֵ
*	��    �Σ�_ch : ��ȡת��ADCһ·���
*	�� �� ֵ: sAdc�����ض�ȡ�Ľ��
*********************************************************************************************************
*/
int16_t AD7606_ReadAdc(uint8_t _ch)
{
	int16_t sAdc;
	
	//DISABLE_INT();	
	sAdc = s_adc_now[_ch];
	//ENABLE_INT();

	return sAdc;
}

//����ͺ��˲�
//�������Ը��ž������õ�����
//���ڲ���Ƶ�ʽϸ߳���
#define fit_a 250  
u16 OneFactorialFiler_1( u16 now_value )
{  
	static  u16 last_value = 0;
	u16 fit_NewValue = 0;
	u32 ReturnValue = 0;
	fit_NewValue = now_value;
	ReturnValue = (255-fit_a)*fit_NewValue + fit_a*last_value;
	ReturnValue /= 255;
	last_value = fit_NewValue;
	return( (u16)ReturnValue );
}
u16 OneFactorialFiler_2( u16 now_value )
{  
	static  u16 last_value = 0;
	u16 fit_NewValue = 0;
	u32 ReturnValue = 0;
	fit_NewValue = now_value;
	ReturnValue = (255-fit_a)*fit_NewValue + fit_a*last_value;
	ReturnValue /= 255;	
	last_value = 	fit_NewValue;
	return( (u16)ReturnValue );
}

/*
*********************************************************************************************************
*	�� �� ��: AD7606_Mak
*	����˵��: ��������������
*	��    �Σ���
*	�� �� ֵ: ��
*********************************************************************************************************
*/
void AD7606_Count(void)
{
	uint8_t i;
	int16_t adc;

	for (i = 0;i < CH_NUM; i++)
	{	
		s_dat[i] = AD7606_ReadAdc(i);
	/* 
		32767 = 5V , ��������ֵ��ʵ�ʿ��Ը���5V��׼��ʵ��ֵ���й�ʽ���� 
		volt[i] = ((int16_t)dat[i] * 5000) / 32767;	����ʵ�ʵ�ѹֵ�����ƹ���ģ�������׼ȷ�������У׼            
		volt[i] = dat[i] * 0.3051850947599719
	*/
		
		adc = s_dat[i];
//		s_volt[i] = ((adc*10000) / 33960)*5;
		s_volt[i] = ((adc*1000) / 32767)*5;
		
		s_volt[i] = OneFactorialFiler_1(s_volt[i]);
		s_volt[i] = OneFactorialFiler_2(s_volt[i]);
	}
	
}

void AD7606_Init(void)  //  ad7606ģ���ʼ��
{
	bsp_InitSPIBus();
	AD7606_ConfigGPIO();	  /* ����GPIO */
	bsp_spi_InitAD7606();	  /* ����AD7606���õ�GPIO */
}


#include "motor.h"


TIM_HandleTypeDef TIM2_Handler;         //��ʱ��PWM��� 
TIM_OC_InitTypeDef TIM2_CHAllHandler;	//��ʱ��ͨ�����

/***************************************************************************
�������ܣ�	TIM��PWM��ʼ��(����4·)
��ڲ�����	arr���Զ���װֵ
			psc��ʱ��Ԥ��Ƶ��
���ز�����	��
****************************************************************************/
void TIM2_PWM_Init(u16 arr,u16 psc)
{ 
    TIM2_Handler.Instance=TIM2;            //��ʱ��
    TIM2_Handler.Init.Prescaler=psc;       //��ʱ����Ƶ
    TIM2_Handler.Init.CounterMode=TIM_COUNTERMODE_UP;//���ϼ���ģʽ
    TIM2_Handler.Init.Period=arr;          //�Զ���װ��ֵ
    TIM2_Handler.Init.ClockDivision=TIM_CLOCKDIVISION_DIV1;
    HAL_TIM_PWM_Init(&TIM2_Handler);       //��ʼ��PWM
    
    TIM2_CHAllHandler.OCMode=TIM_OCMODE_PWM1; //ģʽѡ��PWM1
    TIM2_CHAllHandler.Pulse=arr/2;            //���ñȽ�ֵ,��ֵ����ȷ��ռ�ձȣ�Ĭ�ϱȽ�ֵΪ�Զ���װ��ֵ��һ��,��ռ�ձ�Ϊ50%
    TIM2_CHAllHandler.OCPolarity=TIM_OCPOLARITY_HIGH; //����Ƚϼ���
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_1);//����TIMͨ��
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_2);//����TIMͨ��
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_3);//����TIMͨ��
    HAL_TIM_PWM_ConfigChannel(&TIM2_Handler,&TIM2_CHAllHandler,TIM_CHANNEL_4);//����TIMͨ��
	
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_1);//����PWMͨ��
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_2);//����PWMͨ��
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_3);//����PWMͨ��
    HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_4);//����PWMͨ��
	
//	HAL_TIM_PWM_Start(&TIM2_Handler,TIM_CHANNEL_ALL);//???
}


//��ʱ���ײ�������ʱ��ʹ�ܣ���������
//�˺����ᱻHAL_TIM_PWM_Init()����
//htim:��ʱ�����
void HAL_TIM_PWM_MspInit(TIM_HandleTypeDef *htim)
{
    GPIO_InitTypeDef GPIO_Initure;
	__HAL_RCC_TIM2_CLK_ENABLE();			//ʹ�ܶ�ʱ��
    __HAL_RCC_GPIOA_CLK_ENABLE();			//����GPIOAʱ��
	
    GPIO_Initure.Pin=GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3;	//pin
    GPIO_Initure.Mode=GPIO_MODE_AF_PP;			//�����������
    GPIO_Initure.Pull=GPIO_PULLUP;				//����
    GPIO_Initure.Speed=GPIO_SPEED_HIGH;			//����
	GPIO_Initure.Alternate= GPIO_AF1_TIM2;		//����ΪTIM
    HAL_GPIO_Init(GPIOA,&GPIO_Initure);
}



/**************************************************************************************
�������ܣ�	С������
��ڲ�����	LeftPwm����ߵ��PWMֵ
			RightPwm���ұߵ��PWMֵ
			Mode������ģʽ
���ز�����	��
***************************************************************************************/
void Motor_Control(u16 LeftPwm,u16 RightPwm,u8 Mode)
{	
	switch(Mode)
	{
		/*ǰ��*/
		case FRONT:	
				PWM1 = LeftPwm;
				PWM2 = 0;
				PWM3 = RightPwm;
				PWM4 = 0;
			break;
		
		/*����*/
		case REAR:	
				PWM1 = 0;
				PWM2 = LeftPwm;
				PWM3 = 0;
				PWM4 = RightPwm;
			break;
		
		/*��ת*/
		case LEFT:	
				PWM1 = 0;
				PWM2 = LeftPwm+1000;
				PWM3 = RightPwm+1000;
				PWM4 = 0;
			break;
		
		/*��ת*/
		case RIGHT:
				PWM1 = LeftPwm+1000;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = RightPwm+1000;
			break;
		
		/*ֹͣ*/
		case STOP:
				PWM1 = 0;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = 0;
			break;
		
		default:PWM1 = 0;
				PWM2 = 0;
				PWM3 = 0;
				PWM4 = 0;
	}
}



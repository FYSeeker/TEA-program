#include "handle.h"


/******************************************************************
*��������:	Sinistrogyration
*��������:	�������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Handle_delay(u16 t)
{
	while(t--)
	{
		Camera_refresh();	//����LCD��ʾ
		delay_us(500);
	}
}


/******************************************************************
*��������:	Sinistrogyration
*��������:	�������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Sinistrogyration(void)
{	
	TIM_SetCompare3(TIM8,1300);
	Handle_delay(500);
}


/******************************************************************
*��������:	Dextroversion
*��������:	�������
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Dextroversion(void)
{	
	TIM_SetCompare3(TIM8,550);
	Handle_delay(500);
}


/******************************************************************
*��������:	Steering_Reset
*��������:	�����λ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Steering_Reset(void)
{	
	TIM_SetCompare3(TIM8,1000);
}


/******************************************************************
*��������:	Conveyor_Advance
*��������:	���ʹ���ǰ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Conveyor_Advance(void)
{
	TIM_SetCompare1(TIM3,5400);
	TIM_SetCompare2(TIM3,0);
	
	Conveyor_Run = 1;	//��Ǵ��ʹ�����
}


/******************************************************************
*��������:	Conveyor_Recede
*��������:	���ʹ����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Conveyor_Recede(void)
{
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,5400);
	
	Conveyor_Run = 1;	//��Ǵ��ʹ�����
}


/******************************************************************
*��������:	Conveyor_Stop
*��������:	ֹͣ���ʹ�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Conveyor_Stop(void)
{
	TIM_SetCompare1(TIM3,0);
	TIM_SetCompare2(TIM3,0);
	
	Conveyor_Run = 0;
}


/******************************************************************
*��������:	Push_1
*��������:	��Ʒ�����ұߣ�1��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Push_1(void)
{
	u16 Distance=0;		//���ʹ�����
	
	/*��Ʒ�����ұߣ�1��*/
	Sinistrogyration();		//�������
	Distance = TIM4->CNT;	//��¼���ʹ�λ��
	Conveyor_Advance();		//�������ʹ�
	while((TIM4->CNT < (Distance+Variate))&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//����LCD��ʾ
	}
	Conveyor_Stop();		//ֹͣ���ʹ�
	Dextroversion();		//�������
	Steering_Reset();		//�����λ
	Conveyor_Recede();		//���ʹ�����
	while((TIM4->CNT > Distance)&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//����LCD��ʾ
	}
	Conveyor_Stop();		//ֹͣ���ʹ�
	Handle_delay(200);	
}


/******************************************************************
*��������:	Push_2
*��������:	��Ʒ����ǰ�棬2��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Push_2(void)
{
	u16 Distance=0;		//���ʹ�����
	
	/*��Ʒ����ǰ�棬2��*/
	Sinistrogyration();		//�������
	Distance = TIM4->CNT;	//��¼���ʹ�λ��
	Conveyor_Advance();		//�������ʹ�
	while((TIM4->CNT < (Distance+Variate+135))&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//����LCD��ʾ
	}
	Conveyor_Stop();		//ֹͣ���ʹ�
	Handle_delay(300);
	Conveyor_Recede();		//���ʹ�����
	while((TIM4->CNT > (Distance))&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//����LCD��ʾ
	}
	Conveyor_Stop();		//ֹͣ���ʹ�
	Steering_Reset();		//�����λ	
	Handle_delay(200);	
}


/******************************************************************
*��������:	Push_3
*��������:	��Ʒ������ߣ�3��
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Push_3(void)
{
	u16 Distance=0;		//���ʹ�����
	
	/*��Ʒ������ߣ�3��*/
	Dextroversion();		//�������	
	Distance = TIM4->CNT;	//��¼���ʹ�λ��
	Conveyor_Advance();		//�������ʹ�
	while((TIM4->CNT < (Distance+Variate))&&(TIM4->CNT<3000))
	{			
		Camera_refresh();	//����LCD��ʾ
	}
	Conveyor_Stop();		//ֹͣ���ʹ�		
	Sinistrogyration();		//�������
	Steering_Reset();		//�����λ
	Conveyor_Recede();		//���ʹ�����
	while((TIM4->CNT > Distance)&&(TIM4->CNT<3000))
	{
		Camera_refresh();	//����LCD��ʾ
	}
	Conveyor_Stop();		//ֹͣ���ʹ�
	Handle_delay(200);	
}


/******************************************************************
*��������:	Result_Handler
*��������:	���������,�����ж�����ɫ���������Ϣ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Result_Handler(u8 dat)
{	
	/*�����ɫ���*/
	switch(dat)
	{
		case 1:printf("��ɫ\r\n");
			break;
		
		case 2:printf("��ɫ\r\n");
			break;
		
		case 3:printf("��ɫ\r\n");
			break;
		
		case 4:printf("��ɫ\r\n" );
			break;
		
		case 5:printf("��ɫ\r\n");
			break;
		
		case 6:printf("��ɫ\r\n");
			break;
		
		case 7:printf("��ɫ\r\n");
			break;
		
		default:printf("����\r\n");	
	}
	printf("\r\n");
	
	if(dat==5)		//��ɫ
	{
		Push_1();
	}
	else if(dat==2)	//��ɫ
	{	
		Push_2();
	}
	else
	{	
		Push_2();
	}
}


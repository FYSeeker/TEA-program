#include "control.h"
#include "main.h"

/*
 - ҡ�˲ɼ�ֵ
 - ADCX[0],ADCX[1] ��ҡ�� X Y
 - ADCX[2],ADCX[3] ��ҡ�� X Y 
*/
float ADCX[4]; 

/*
 - ����ֵ
 - ��Ӧ����THROTTLR�Ӽ���
 - 0-4  Ϊ�� -(5-1)
 - 6-10 Ϊ�� +(1-5)
*/
u16 THROTTLR;

u8 Before_after;  	// ǰ���־  1-ǰ��  2-����
u8 Right_left;  	// ���ұ�־  1-��  2-��
u8 Rotation;  		// ��ת��־  1-��  2-��

/*
 - ������־λ
 - 0 - ����
 - 1 - ����
*/
u8  ARMED=0;


/***********************************
* NRFDate[0]��
*			 1--ǰ
*			 2--��
* NRFDate[1]��
*			 1--��
*			 2--��
* NRFDate[2]��
*			 1--����
*			 2--����
* NRFDate[3]��
*			 9--����
*			 6--����
* NRFDate[3]��
*			 1--ͨ����
*			 ����--ͨ�ŶϿ�
************************************/
u8 NRFSendbuf[5] = {0,0,0,0,1};	// �������ݻ���

void ADC_Gather(void)
{
	u8 i;
	
	for(i=0;i<4;i++)
	{
		ADCX[i] = Get_Adc(i);
		ADCX[i] /= 400.0;
	}
	
	THROTTLR = (u8)ADCX[1]; 		// ����
	Rotation = (u8)ADCX[0];  		// ��ת
	Before_after = (u8)ADCX[3]; 	 //ǰ��
	Right_left = (u8)(ADCX[2]+0.4);  //����
}

void KEY_Control(void)
{
	u8 key=KEY_Scan(0);
	
	if(key==KEY0_PRES)ARMED=1;  // ����
	else if(key==KEY1_PRES)ARMED=0;  // ����
}

void NRF_SendData(void)
{
	if(Before_after>5)NRFSendbuf[0]=1;  // ǰ��
	else if(Before_after<5)NRFSendbuf[0]=2; 
	else NRFSendbuf[0]=0;
	
	if(Right_left<5)NRFSendbuf[1]=1;  // ����
	else if(Right_left>5)NRFSendbuf[1]=2; 
	else NRFSendbuf[1]=0;
	
	if(THROTTLR>5)NRFSendbuf[2]=1;  // ����
	else if(THROTTLR<5)NRFSendbuf[2]=2; 
	else NRFSendbuf[2]=0;
	
	if(ARMED)NRFSendbuf[3]=6;	// ����
	else NRFSendbuf[3]=9;		// ����
	
	NRF24L01_TxPacket(NRFSendbuf);	// ��������		
}

int myabs(int a)  // ����ֵ����
{ 		   
	int temp;
	if(a<0)  temp=-a;  
	else temp=a;
	return temp;
}




#include "blance.h"
#include "usart.h"
#include "inv_mpu.h"

volatile float roll[2];		//ŷ����

u8 ARMED = 0;		//����������־λ
u8 Accelerator=1;	//�ٶȵ�λ��1��2��3��4

PID BlancePid	= {0.03, 0.6, 0.2};		//ƽ�⻷pid


/********************************************************************
*
* �õ�����ֵ
*
*********************************************************************/
float maybe(float pwm)				
{
	if(pwm<0)pwm = -pwm;
	
	if(pwm>7200)pwm = 7200;		//�޷�����ֹ����7200
	
	return pwm;
}


/********************************************************************
*
* ƽ�⻷:PD����
* PID����ʽ:IncresePwm = Kp*(E(k)-E(k-1))+Ki*E(k)+Kd*(E(k)-2*E(k-1)+E(k-2))
* PIDλ��ʽ:	PosPwm = Kp*e(k)+Ki*��e(k)+Kd[e��k��-e(k-1)]
*
*********************************************************************/
float Blance(float pitch1, float gyro_balance)
{
	float blance_pwm;
	
	static float Gyro;				//��ǰ���ٶ�
	static float Last_Gyro;			//�ϴν��ٶ�
	static float Gyro_Change;		//���ٶȱ仯�ʣ�΢�֣�
	
	Last_Gyro = Gyro;				//��¼�ϴν��ٶ�
	Gyro = gyro_balance - 0;		//���㱾�ν��ٶ�
	Gyro_Change = Gyro - Last_Gyro;	//����΢��
	
	blance_pwm = BlancePid.P*gyro_balance + BlancePid.I*pitch1 + BlancePid.D*Gyro_Change;
	
	if (blance_pwm > 20) blance_pwm = 20;			//pwm�޷�
	else if(blance_pwm < -20) blance_pwm = -20;
	
	return -blance_pwm;
}


/********************************************************************
*
* �������
*
*********************************************************************/
void start_machine(short leftpwm, short rightpwm, float pitch_1,float pitch_2)
{
//	static u8 x=0;
//	static short SetSpeed=2;
	
	if(maybe(pitch_1)<50.0 && maybe(pitch_2)<50.0 && ARMED)		// ���ڽ���  �����û�����������
	{
		if(leftpwm > 0) DIR_L = 1;	//���ַ����ж�
		else DIR_L = 0;
		
		if(rightpwm > 0) DIR_R = 0;	//���ַ����ж�
		else DIR_R = 1;
		
		leftpwm = maybe(leftpwm);	//�õ�pwm����ֵ
		rightpwm = maybe(rightpwm);		
		
//		if(maybe(pitch_1)>5 && maybe(pitch_2)>5)
//		{	
//			if(x>1)
//			{
//				x=0;
//				
//				/*�˳����ȷ�����Ű�ȫ*/
//				switch(Accelerator)					
//				{
//					case 1:if(SetSpeed<300) SetSpeed += 1;
//						break;
//					case 2:if(SetSpeed<400) SetSpeed += 1;
//						break;
//					case 3:if(SetSpeed<500) SetSpeed += 1;
//						break;
//					case 4:if(SetSpeed<600) SetSpeed += 1;
//						break;
//					default:if(SetSpeed<400) SetSpeed += 1;
//				}
//			}
//			x++;
//			
//			leftpwm	+= SetSpeed;
//			rightpwm += SetSpeed;
//		}
//		else
//		{
//			
//			if(SetSpeed>0) SetSpeed -= 1;
//		}
		
		PWM_L = leftpwm;			//������յõ���pwmֵ
		PWM_R = rightpwm;
	}
	else	//�رյ��
	{
		PWM_L = 0;
		PWM_R = 0;
	}
}


/********************************************************************
*
* �жϴ�����
*
*********************************************************************/
void TIM1_UP_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
	{	
		static short LeftPwm=0, RightPwm=0;
	
		if(ARMED)
		{
			LeftPwm  += Blance(roll[0],Gyro[0][0]);		//����ƽ�⻷
			RightPwm += Blance(roll[1],Gyro[1][0]);		//����ƽ�⻷
			
			/*����޷�*/
			if(LeftPwm>1500) LeftPwm = 1500;
			else if(LeftPwm<-1500) LeftPwm = -1500;
			
			if(RightPwm>1500) RightPwm = 1500;
			else if(RightPwm<-1500) RightPwm = -1500;
			
			start_machine(LeftPwm, RightPwm, roll[0],roll[1]);	//�������
			
//			printf("\r\n%d\t%d\r\n",LeftPwm,RightPwm);
		}
		else	//�رյ��
		{
			PWM_L = 0;
			PWM_R = 0;
			
			LeftPwm = 0;
			RightPwm = 0;
		}
		
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

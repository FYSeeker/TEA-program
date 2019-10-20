#include "blance.h"
#include "usart.h"
#include "inv_mpu.h"

volatile float roll[2];		//ŷ����

u8 ARMED = 0;		//����������־λ
u8 Accelerator=4;	//�ٶȵ�λ��1��2��3��4

PID BlancePid	= {100, 0, 0.05};		//ƽ�⻷pid


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
	float nowError,blance_pwm;
	static float  ErrorIntegral;		//������
	
	nowError = pitch1 - 0;
	ErrorIntegral += nowError;
	
	/*�����޷�*/
	if(ErrorIntegral>1000) ErrorIntegral = 1000;
	else if(ErrorIntegral<-1000) ErrorIntegral = -1000;
	
	blance_pwm = nowError * BlancePid.P + ErrorIntegral * BlancePid.I + gyro_balance * BlancePid.D;
	
	/*PWM�޷�*/
	if (blance_pwm > 1500)blance_pwm = 1500;			
	else if (blance_pwm < -1500)blance_pwm = -1500;
	
	return -blance_pwm;
}


/********************************************************************
*
* �������
*
*********************************************************************/
void start_machine(short leftpwm, short rightpwm, float pitch_1,float pitch_2)
{
	static u8 x=0;
	static short SetSpeed=0;
	
	if(maybe(pitch_1)<50.0 && maybe(pitch_2)<50.0 && ARMED)		// ���ڽ���  �����û�����������
	{	
		
		if(leftpwm > 0) DIR_L = 1;	//���ַ����ж�
		else DIR_L = 0;
		
		if(rightpwm > 0) DIR_R = 0;	//���ַ����ж�
		else DIR_R = 1;
		
		leftpwm = maybe(leftpwm);	//�õ�pwm����ֵ
		rightpwm = maybe(rightpwm);		
		
		if(maybe(pitch_1)>5 && maybe(pitch_2)>5)
		{	
			if(x>1)
			{
				x=0;
				
				/*�˳����ȷ�����Ű�ȫ*/
				switch(Accelerator)					
				{
					case 1:if(SetSpeed<300) SetSpeed += 1;
						break;
					case 2:if(SetSpeed<400) SetSpeed += 1;
						break;
					case 3:if(SetSpeed<500) SetSpeed += 1;
						break;
					case 4:if(SetSpeed<600) SetSpeed += 1;
						break;
					default:if(SetSpeed<400) SetSpeed += 1;
				}
			}
			x++;		
		}
		else
		{		
			if(SetSpeed>0) SetSpeed -= 1;
		}
		
		PWM_L = leftpwm + SetSpeed;			//������յõ���pwmֵ
		PWM_R = rightpwm + SetSpeed;
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
		static short Left_LastPwm=0,Right_LastPwm=0;
	
		if(ARMED)
		{			
			LeftPwm  = Blance(roll[0],Gyro[0][0]);		//����ƽ�⻷
			RightPwm = Blance(roll[1],Gyro[1][0]);		//����ƽ�⻷
			
			LeftPwm = Left_LastPwm*0.9 + LeftPwm*0.1;
			RightPwm = Right_LastPwm*0.9 + RightPwm*0.1;
			
			start_machine(LeftPwm, RightPwm, roll[0],roll[1]);	//�������	
			
			Left_LastPwm = LeftPwm;
			Right_LastPwm = RightPwm;		
		}
		else	//�رյ��
		{
			PWM_L = 0;
			PWM_R = 0;
		}
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}

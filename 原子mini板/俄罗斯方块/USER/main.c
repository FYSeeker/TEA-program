
/***************
@ brief :  TEARS.C
@ auther:  KXT
@ date  :  2016.7.23
***************/

/*************************
@ WKUP -- ���� ��λ
@ KEY0 -- ���� ȷ��
@ KEY1 -- ��״ ��ֵ
@ ���Ͻ� -- ������
@ ���½� -- ����
**************************/

# include "stm32f10x.h"
# include "led.h"
# include "key.h"
# include "delay.h"
# include "lcd.h"
# include "touch.h" 
# include "usart.h"
# include "tears.h"
# include "timer.h"
# include "interface.h"

extern u32 sh_block[][4];
extern u32 dir_lines[][3];

extern _Shape NewShape;
u32 Interface_ChangeFLAG = 1;

int main(void)
{
	static u32 task1 = 0;
	static u32 down_Addspeed = 0;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);// �����ж����ȼ�����2
	delay_init();	    	 //��ʱ������ʼ��	  
 	LED_Init();			     //LED�˿ڳ�ʼ��
	KEY_Init();              //�����˿ڳ�ʼ��
	uart_init(38400);        //���ڳ�ʼ��
	tp_dev.init();			 //��������ʼ��
	TIME_Init(999,71);       //��ʱ����ʼ��
	LCD_Init();              //LCD��ʼ��
	
	Start_Interface();    //������Ϸǰ��ѡ������Լ�
	Int_Block();          //��Block���г�ʼ��

   	while(1)
	{
		Update_UserLevel(User_Date());    //�����û��÷������µȼ�
		if(1 == Interface_ChangeFLAG)
		{
			Select_StartSpeed();
		}
		else if(0 == Interface_ChangeFLAG)
		{
			Interface_ChangeFLAG = 100;          //��Interface_ChangeFLAG=100������װ�ػ��棬�����������ѭ��
			Game_Interface();
		}	
		else if(100 == Interface_ChangeFLAG)   //�ɴ˿�ʼ����Ϸ���еĳ���
		{	
			down_Addspeed = 0;
			KEY_Function();  //����
			tp_dev.scan(0);  //ɨ�败����
			if(tp_dev.sta&TP_PRES_DOWN)			//������������
			{	
				if(tp_dev.x[0]<240&&tp_dev.y[0]<320)
				{
					if(tp_dev.y[0]>290 &&tp_dev.x[0]>210)  //����
					{
						down_Addspeed = 70-Set_level(User_Date());
						delay_ms(30);
					}
					if(tp_dev.y[0]<30 &&tp_dev.x[0]>210)  
					{
						A_Key_FunCation();  //����
					}
					LED1=!LED1;
				}
			}
			if(Systick_Count > task1+Set_level(User_Date())+down_Addspeed)
			{
				LED0 = !LED0;
				task1 = Systick_Count;
				Shape_Down_Move(&NewShape);  //ͼ���ƶ�����
			}		
			Shape_Inform(&NewShape,Draw);  //ͼ�ε����ɻ��߲���
			Load_NextAndSetflag(&NewShape,sh_block,dir_lines);  //���߽����겢�趨�ƶ���־
			Show_UserDate(User_Date());  //��ʾ�û��ķ�������Ϸ�ȼ���Ϣ
		}
	}		
}


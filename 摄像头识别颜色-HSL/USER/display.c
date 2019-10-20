#include "display.h"
#include "ov7670.h"
#include "lcd.h"
#include "timer.h"
#include "exti.h"
#include "usart.h"
#include "key.h"
#include "tpad.h"
#include "discern.h"

u8 Discern_Flag=0;	//��ɫʶ���־
u8 over=0;			//�ж�������ʾ
u8 COLOR=0;			//�ж����

/******************************************************************
*��������:	Camera_refresh
*��������:	��������ͷ��ʾ
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Camera_refresh(void)
{
	u32 i;
 	u16 color;								//����ͷ����
	u16 Color_1[Sampling_Number];			//���ÿ����������
	static u16 Color_2[Sampling_Number];	//���ÿ֡����
	static u16 Result[Verdict_Nunber];		//����жϽ��
	static u8 Frame_Number=0;				//��¼�ɼ�֡��
	static u8 Discern_Number=0;				//ʶ�����
	
	if(ov_sta)	//��֡�жϸ���
	{
		LCD_Scan_Dir(U2D_L2R);		//���ϵ���,������  
		
		LCD_WriteRAM_Prepare();     //��ʼд��GRAM	
		OV7670_RRST=0;				//��ʼ��λ��ָ�� 
		OV7670_RCK_L;
		OV7670_RCK_H;
		OV7670_RCK_L;
		OV7670_RRST=1;				//��λ��ָ����� 
		OV7670_RCK_H;
		
		for(i=0;i<76800;i++)
		{
			/*��ȡ����ͷ��һ�����ص�*/
			READ_FIFO_PIXEL(RGB565);
			LCD->LCD_RAM=color; 	//д������

			if(Discern_Flag)		//��ɫʶ���־
			{
				/*�ɼ��ض����ص���ɫ*/
				switch(i)
				{
					case 35350:Color_1[0]=color;break;	//����(110,150)
					case 35360:Color_1[1]=color;break;	//����(110,160)
					case 35370:Color_1[2]=color;break;	//����(110,170)
					
					case 36950:Color_1[3]=color;break;	//����(115,150)
					case 36960:Color_1[4]=color;break;	//����(115,160)
					case 36970:Color_1[5]=color;break;	//����(115,170)
					
					case 38550:Color_1[6]=color;break;	//����(120,150)
					case 38560:Color_1[7]=color;break;	//����(120,160)
					case 38570:Color_1[8]=color;break;	//����(120,170)
					
					case 40150:Color_1[9]=color;break;	//����(125,150)
					case 40160:Color_1[10]=color;break;	//����(125,160)
					case 40170:Color_1[11]=color;break;	//����(125,170)
					
					case 41750:Color_1[12]=color;break;	//����(130,150)
					case 41760:Color_1[13]=color;break;	//����(130,160)
					case 41770:Color_1[14]=color;break;	//����(130,170)
				}
			}
		}
				
		LCD_DrawRectangle(140,100,180,140);	//��ʾ���
		
		if(Discern_Flag)	//��ɫʶ��
		{			
			Result[Discern_Number++] = Identify_Color(Average_Filter(Color_1));	//���ÿ֡����
			
			if(Discern_Number > Verdict_Nunber-1)	//�ж�Verdict_Nunber��
			{
				for(i=0;i<Verdict_Nunber;i++) printf("%d\t",Result[i]);
				printf("\r\n");
				
				COLOR = Maximum(Result);
				
				Discern_Number = 0;		//�жϴ�����0
				Discern_Flag = 0;		//ʶ���־	
				over = 1;				//�жϽ���
			}
		}
		
 		ov_sta=0;					//����֡�жϱ��
		LCD_Scan_Dir(DFT_SCAN_DIR);	//�ָ�Ĭ��ɨ�跽��
		
		if(Discern_Flag)	//��ɫʶ��
		{
			LCD_ShowString(60,270,120,24,24,(u8*)"Discern...");	//ʶ����ʾ
		}
	} 
}

/******************************************************************
*��������:	Average_Filter
*��������:	��ֵƽ���˲�
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
u32 Average_Filter(u16* array)
{
	u8 i;
	u32 temp=0;
	
	Data_Sort1(array);	//ð�ݷ����򣬴�С����	
	
	for(i=3; i<Sampling_Number-3; i++)
	{	
		temp += array[i];
	}	
		
	return temp/(Sampling_Number-6);
}
/******************************************************************
*��������:	Data_Sort
*��������:	ð�ݷ����򣬴�С����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Data_Sort1(u16* array)
{
	u8 m, n;
	u32 temp;
	
	for(m=0; m<Sampling_Number-1; m++)
	{
		for(n=0; n<Sampling_Number-1-m; n++)
		{
			if(array[n] > array[n+1])
			{
				temp = array[n];
				array[n] = array[n+1];
				array[n+1] = temp;
			}
		}
	}
}


/******************************************************************
*��������:	Data_Sort2
*��������:	���򣬴�С����
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
u16 Data_Sort2(u16* array,u16 number)
{
	u8 m, n;
	u32 temp;
	
	for(m=0; m<number-1; m++)
	{
		
		for(n=m+1; n<number; n++)
		{
			if(array[m] > array[n])
			{
				temp = array[m];
				array[m] = array[n];
				array[n] = temp;
			}
		}
	}
	
	return array[number];
}


u16 Maximum(u16* array)
{
	u16 m,n;
	u16 x[7];
	
	for(n=0;n<Verdict_Nunber-1;n++)
	{
		if(array[n]==1) x[0]++;
		else if(array[n]==2) x[1]++;
		else if(array[n]==3) x[2]++;
		else if(array[n]==4) x[3]++;
		else if(array[n]==5) x[4]++;
		else if(array[n]==6) x[5]++;
		else if(array[n]==7) x[6]++;
	}
	
	return Data_Sort2(x,7);
}


/******************************************************************
*��������:	Set_OV7670
*��������:	��������OV7670
*��������:	��
*�� �� ֵ:	��
*******************************************************************/
void Set_OV7670()
{
 	const u8* LMODE_TBL[6] ={(u8*)"Auto",(u8*)"Sunny",(u8*)"Cloudy",(u8*)"Office",(u8*)"Home"};		//5�ֹ���ģʽ

	u8 key;	 
	static u8 msgbuf[15];				//��Ϣ������
	static u8 tm=0;
	
	key = KEY_Scan(0);//��֧������
	if(key)
	{
		tm=10;
		switch(key)
		{				    
			case KEY0_PRES:	//�ƹ�ģʽLight Mode
				lightmode++;
				if(lightmode>4)lightmode=0;
				OV7670_Light_Mode(lightmode);
				sprintf((char*)msgbuf,"%s",LMODE_TBL[lightmode]);
				break;
			
			case KEY1_PRES:	//���Ͷ�Saturation
				saturation++;
				if(saturation>4)saturation=0;
				OV7670_Color_Saturation(saturation);
				sprintf((char*)msgbuf,"Saturation:%d",(signed char)saturation-2);
				break;
			
			case KEY2_PRES:	//����Brightness				 
				brightness++;
				if(brightness>4)brightness=0;
				OV7670_Brightness(brightness);
				sprintf((char*)msgbuf,"Brightness:%d",(signed char)brightness-2);
				break;
			
			case WKUP_PRES:	//�Աȶ�Contrast			    
				contrast++;
				if(contrast>4)contrast=0;
				OV7670_Contrast(contrast);
				sprintf((char*)msgbuf,"Contrast:%d",(signed char)contrast-2);
				break;
		}
	}	 
	
	if(tm)
	{
		LCD_ShowString((lcddev.width-240)/2+30,(lcddev.height-320)/2+60,200,16,16,msgbuf);
		tm--;
	}
}

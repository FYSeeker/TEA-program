#include "my_gui.h"


/***********ȫ�ֱ���************/

s8 Keynum=0;  //��������
u8 keynum[5];  //����������Ϣ
u8 Radix=0, Keyflag=0, Touflag=20, Keytemp=0, Ensure=0, Addflag=0;  //С������, �����������, �������, �������, ȷ�����, �ۼӱ��
float quality=0, quaflag=0, scale=0, quazero=0, quatemp=0;  //�������������, ȥƤ��־, �������, ��������
float uprice=0,tprice=0, addtprice=0;  //���ۣ��ܼ�, �ۼӽ��




void Basic_Interface(void)  //��������
{
	LCD_Clear(BLACK);
	BACK_COLOR=BLACK;
	
	LCD_Fill(0,0,152,34,YELLOW);
	
	POINT_COLOR=RED;
	LCD_ShowWord(0,2,3,30,1);//��	
	LCD_ShowWord(30,2,4,30,1);//��
	LCD_ShowWord(60,2,0,30,1);//��	
	LCD_ShowWord(90,2,1,30,1);//��
	LCD_ShowWord(120,2,2,30,1);//��
	
	LCD_ShowWord(240-52,2,13,24,1);//��
	LCD_ShowWord(240-28,2,14,24,1);//��
	
	POINT_COLOR=GRAY;
	LCD_ShowWord(20,40,0,24,1);//��	
	LCD_ShowWord(44,40,1,24,1);//ѹ
	LCD_ShowString(68,40,132,24,24,":      V");//:    
	
	LCD_ShowWord(20,70,2,24,1);//��	
	LCD_ShowWord(44,70,3,24,1);//��
	LCD_ShowString(68,70,132,24,24,":      g");//:    g
	
	LCD_ShowWord(20,100,4,24,1);//��	
	LCD_ShowWord(44,100,5,24,1);//��
	LCD_ShowString(68,100,120,24,24,":       /g");//:    Ԫ/g
	LCD_ShowWord(140,100,7,24,1);//Ԫ
	
	LCD_ShowWord(20,130,6,24,1);//��	
	LCD_ShowWord(44,130,5,24,1);//��
	LCD_ShowChar(68,130,':',24,1);//:
	LCD_ShowWord(140,130,7,24,1);//Ԫ
	
	LCD_ShowChar(168,303,'@',16,1);//@
	LCD_ShowWord(176,303,0,16,1);//ģ	
	LCD_ShowWord(192,303,1,16,1);//��
	LCD_ShowWord(208,303,2,16,1);//һ
	LCD_ShowWord(224,303,3,16,1);//��
}


void LCD_Keyboard(void)  //����
{
	POINT_COLOR=LBBLUE;
	LCD_DrawRectangle(0,160,240,304);//��
	LCD_DrawLine(55,160,55,304);//����
	LCD_DrawLine(110,160,110,304);
	LCD_DrawLine(165,160,165,304);
	LCD_DrawLine(0,196,240,196);//����
	LCD_DrawLine(0,232,240,232);
	LCD_DrawLine(0,268,240,268);
	
	
	POINT_COLOR=LIGHTGREEN;
	LCD_ShowChar(18,166,'7',24,1);
	LCD_ShowChar(73,166,'8',24,1);
	LCD_ShowChar(128,166,'9',24,1);
	
	LCD_ShowChar(18,202,'4',24,1);
	LCD_ShowChar(73,202,'5',24,1);
	LCD_ShowChar(128,202,'6',24,1);
	
	LCD_ShowChar(18,238,'1',24,1);
	LCD_ShowChar(73,238,'2',24,1);
	LCD_ShowChar(128,238,'3',24,1);
	
	LCD_ShowChar(18,274,'.',24,1);
	LCD_ShowChar(73,274,'0',24,1);

	
	POINT_COLOR=WHITE;
	LCD_ShowWord(112,274,11,24,1);//ȷ
	LCD_ShowWord(136,274,12,24,1);//��
	
	LCD_ShowString(182,165,36,24,24,"<--");
	LCD_ShowString(183,166,36,24,24,"<--");
	LCD_ShowString(182,167,36,24,24,"<--");
	
	LCD_ShowWord(182,202,15,24,1);//��
	LCD_ShowWord(206,202,16,24,1);//��
		
	LCD_ShowWord(182,238,9,24,1);//ȥ
	LCD_ShowWord(206,238,10,24,1);//Ƥ	
	
	LCD_ShowString(180,274,48,24,24,"ZERO");
}


void LCD_Touch(void)  //������
{
	tp_dev.scan(0);
	if(tp_dev.sta&TP_PRES_DOWN)			//������������
	{	
		if(tp_dev.x[0]>0 &&tp_dev.x[0]<240 &&tp_dev.y[0]>160 &&tp_dev.y[0]<304)
		{
			if(tp_dev.x[0]>0 &&tp_dev.x[0]<55 &&tp_dev.y[0]>232 &&tp_dev.y[0]<268)  //1
			{
				LCD_Fill(1,233,54,267,CYAN);  //1
				Touflag = 1;
			}
			else if(tp_dev.x[0]>55 &&tp_dev.x[0]<110 &&tp_dev.y[0]>232 &&tp_dev.y[0]<268)  //2
			{
				LCD_Fill(56,233,109,267,CYAN);  //2
				Touflag = 2;
			}
			else if(tp_dev.x[0]>110 &&tp_dev.x[0]<165 &&tp_dev.y[0]>232 &&tp_dev.y[0]<268)  //3
			{
				LCD_Fill(111,233,164,267,CYAN);  //3
				Touflag = 3;
			}
			else if(tp_dev.x[0]>0 &&tp_dev.x[0]<55 &&tp_dev.y[0]>196 &&tp_dev.y[0]<232)  //4
			{
				LCD_Fill(1,197,54,231,CYAN);  //4
				Touflag = 4;
			}
			else if(tp_dev.x[0]>55 &&tp_dev.x[0]<110 &&tp_dev.y[0]>196 &&tp_dev.y[0]<232)  //5
			{
				LCD_Fill(56,197,109,231,CYAN);  //5
				Touflag = 5;
			}
			else if(tp_dev.x[0]>110 &&tp_dev.x[0]<165 &&tp_dev.y[0]>196 &&tp_dev.y[0]<232)  //6
			{
				LCD_Fill(111,197,164,231,CYAN);  //6
				Touflag = 6;
			}
			else if(tp_dev.x[0]>0 &&tp_dev.x[0]<55 &&tp_dev.y[0]>160 &&tp_dev.y[0]<196)  //7
			{
				LCD_Fill(1,161,54,195,CYAN);  //7
				Touflag = 7;
			}
			else if(tp_dev.x[0]>55 &&tp_dev.x[0]<110 &&tp_dev.y[0]>160 &&tp_dev.y[0]<196)  //8
			{
				LCD_Fill(56,161,109,195,CYAN);  //8
				Touflag = 8;
			}
			else if(tp_dev.x[0]>110 &&tp_dev.x[0]<165 &&tp_dev.y[0]>160 &&tp_dev.y[0]<196)  //9
			{
				LCD_Fill(111,161,164,195,CYAN);  //9
				Touflag = 9;
			}
			else if(tp_dev.x[0]>55 &&tp_dev.x[0]<110 &&tp_dev.y[0]>268 &&tp_dev.y[0]<304)  //0
			{
				LCD_Fill(56,269,109,303,CYAN);  //0
				Touflag = 10;
			}
			else if(tp_dev.x[0]>0 &&tp_dev.x[0]<55 &&tp_dev.y[0]>268 &&tp_dev.y[0]<304)  //.
			{
				LCD_Fill(1,269,54,303,CYAN);  //.
				Touflag = 11;
				Radix = (Keynum+1);
			}
			else if(tp_dev.x[0]>110 &&tp_dev.x[0]<165 &&tp_dev.y[0]>268 &&tp_dev.y[0]<304)  //ȷ��
			{
				LCD_Fill(111,269,164,303,CYAN);  //ȷ��
				if(Addflag)addtprice+=tprice;  //�ۼӽ��
				Touflag = 12;
				Ensure = 1;
			}
			else if(tp_dev.x[0]>165 &&tp_dev.x[0]<240 &&tp_dev.y[0]>160 &&tp_dev.y[0]<196)  //<--
			{
				LCD_Fill(166,161,239,195,CYAN);  //<--
				if(Keynum <= 1)
				{
					keynum[0]=0;
				}
				else if(Radix == Keynum)
				{
					Radix=0;
					Keynum-=2;
				}
				else
				{
					keynum[Keynum-1]=0;
					Keynum-=2;
				}
				LCD_Trends();
				LCD_Keyboard();
				Touflag = 13;
			}
			else if(tp_dev.x[0]>165 &&tp_dev.x[0]<240 &&tp_dev.y[0]>196 &&tp_dev.y[0]<232)  //ADD
			{
				u8 i;  
				LCD_Fill(166,197,239,231,CYAN);  //ADD
				Radix = 0;
				uprice = 0;  //��������
				Addflag = 1;  //����ۼ�
				Touflag = 14;
				for(i=0;i<4;i++)
				{
					keynum[i]=0;
				}
				LCD_Fill(80,70,152,100,BLACK);
				LCD_Fill(80,100,140,190,BLACK);
			}
			else if(tp_dev.x[0]>165 &&tp_dev.x[0]<240 &&tp_dev.y[0]>232 &&tp_dev.y[0]<268)  //ȥƤ
			{
				LCD_Fill(166,233,239,267,CYAN);  //ȥƤ
				LCD_Fill(80,70,152,100,BLACK);
				LCD_Fill(80,100,140,190,BLACK);
				quaflag = quality;  //��¼��һ������ֵ
				scale = 1;
				Touflag = 15;
			}
			else if(tp_dev.x[0]>165 &&tp_dev.x[0]<240 &&tp_dev.y[0]>268 &&tp_dev.y[0]<304)  //ZERO
			{
				u8 i;
				LCD_Fill(166,269,239,303,CYAN);  //ZERO
				LCD_Fill(80,70,152,100,BLACK);
				LCD_Fill(80,100,140,190,BLACK);
				Radix = 0;  	//���С����
				Addflag = 0;  	//�ۼӱ��
				tprice = 0;  	//�ܼ�����
				addtprice = 0;  //�ۼӽ������
				Ensure = 0;  	//ȷ���������
				quality = 0;  	//��������
				quaflag = 0;  	//ȥƤ����
				Keynum = 0;  	//�����������
				Touflag = 16;
				for(i=0;i<4;i++)
				{
					keynum[i]=0;
				}
				goto ZE;
			}
			if((Touflag > 0) && (Touflag <= 9))keynum[Keynum]=Touflag;
			if(Touflag == 10)keynum[Keynum]=0;
			LCD_Keyboard();
			Keytemp = 1;
			Keynum++;
		}
		while(tp_dev.sta&TP_PRES_DOWN)  //�ȴ������ɿ�
			tp_dev.scan(0);
	}
	else if(Keyflag != Keynum)
	{
ZE:		LCD_Trends();
		LCD_Keyboard();
		Touflag = 20;
		Keyflag = Keynum;
	}
}


void LCD_Trends(void)  //������̬��ʾ
{
	switch(Touflag)
	{
		case  1:LCD_Fill(1,233,54,267,BLACK);    break;
		case  2:LCD_Fill(56,233,109,267,BLACK);  break;
		case  3:LCD_Fill(111,233,164,267,BLACK); break;
		case  4:LCD_Fill(1,197,54,231,BLACK);    break;
		case  5:LCD_Fill(56,197,109,231,BLACK);  break;
		case  6:LCD_Fill(111,197,164,231,BLACK); break;
		case  7:LCD_Fill(1,161,54,195,BLACK);    break;
		case  8:LCD_Fill(56,161,109,195,BLACK);  break;
		case  9:LCD_Fill(111,161,164,195,BLACK); break;
		case 10:LCD_Fill(56,269,109,303,BLACK);  break;
		case 11:LCD_Fill(1,269,54,303,BLACK);    break;
		case 12:LCD_Fill(111,269,164,303,BLACK); break;
		case 13:LCD_Fill(166,161,239,195,BLACK); break;
		case 14:LCD_Fill(166,197,239,231,BLACK); break;
		case 15:LCD_Fill(166,233,239,267,BLACK); break;
		case 16:LCD_Fill(166,269,239,303,BLACK); break;
	}
}


void LCD_Input(void)  //������������
{
	POINT_COLOR=WHITE;
	if(Keynum > 5)
	{
		Keynum = 0;
		uprice = 0;
	}
	if(Radix > 4)
	{
		Radix = 0;
	}
	if( (Keytemp == 1 && Touflag <= 10) || Touflag == 11 || Touflag == 13)
	{	
		Keytemp = 0;
		if( Radix == 0 )
		{
			switch(Keynum)
			{
				case 1:uprice=keynum[0];break;
				case 2:uprice=keynum[0]*10+keynum[1];break;
				case 3:uprice=keynum[0]*100+keynum[1]*10+keynum[2];break;
				case 4:uprice=keynum[0]*1000+keynum[1]*100+keynum[2]*10+keynum[3];break;
				case 5:uprice=keynum[0]*10000+keynum[1]*1000+keynum[2]*100+keynum[3]*10+keynum[4];break;
			}
			LCD_ShowNum(80,100,(int)uprice,5,24);
		}
		else 
		{
			switch(Radix-1)
			{
				case 1:uprice=keynum[0];break;
				case 2:uprice=keynum[0]*10+keynum[1];break;
				case 3:uprice=keynum[0]*100+keynum[1]*10+keynum[2];break;
			}
			uprice+= (float)keynum[Radix]/10;
			LCD_ShowNum(128-12*Radix,100,(int)uprice,Radix-1,24);
			LCD_ShowChar(116,100,'.',24,0);//.
			LCD_ShowNum(128,100,(int)(uprice*10)%10,1,24);
		}
	}
}

void LCD_Count(s16 date)  //�����ѹֵ��������
{
	u8 buf[10];
	s16 adcx;
	float temp;
	
	adcx = date;
	temp=(float)adcx*1.0/1000;  //��ѹ
	sprintf((char *)buf,"%1.3f",temp);
	LCD_Fill(140,40,152,100,BLACK);
	LCD_ShowStrings(80,40,60,24,24,buf);
	
	quality=(temp*1000 + 96.3214)/6.2621;  //����

	if(scale == 1)quality-=quaflag;  //ȥƤ
	if(temp == 0){quality=0;LCD_Fill(80,70,140,100,BLACK);}  //�����ѹΪ�㣬����Ϊ��
	sprintf((char *)buf,"%3.1f",quality);
	LCD_ShowStrings(80,70,60,24,24,buf);
	
	if(Ensure == 1)  //ȷ��
	{
		tprice=uprice*quality;  //�ܼ�
		if(uprice == 0)tprice=0;  //�������Ϊ�㣬�ܼ�Ϊ��
		if(Addflag == 0)
		{
			if(tprice/1000 > 1) sprintf((char *)buf,"%d",(int)tprice);
			else sprintf((char *)buf,"%4.1f",tprice);
			LCD_ShowStrings(80,130,60,24,24,buf);	
		}
		else
		{
			if(addtprice/1000 > 1) sprintf((char *)buf,"%d",(int)addtprice);
			else sprintf((char *)buf,"%4.1f",addtprice);
			LCD_ShowStrings(80,130,60,24,24,buf);	
		}			
	}
}


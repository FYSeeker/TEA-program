#include "display.h"


/**����ʱ����ʾ**/
void Display_Time(TimeStruct Time)
{	
	LCD_ShowString(96,230,12,2,24,":");	
	LCD_ShowString(132,230,12,2,24,":");	//��ʾ":"
	
	LCD_ShowxNum(72,230,Time.hour,2,24,0X80);	//��ʾʱ
	LCD_ShowxNum(108,230,Time.min,2,24,0X80);	//��ʾ��
	LCD_ShowxNum(144,230,Time.sec,2,24,0X80);	//��ʾ��
}



/**ģ��ʱ����ʾ**/
void Analog_Timer_Display(void)
{
	u8 i=10, hour_needle;
	End_Coord center_xy,end_xy;
	
	LCD_Draw_Circle(120,100,75);	//����Բ
	LCD_Draw_Circle(120,100,77);	//����Բ
	LCD_Draw_Circle(120,100,2);		//������
	LCD_Draw_Circle(120,100,1);		//������
	
	for(i=0;i<60;i++)		//��ȡ�̶������겢�����̶���
	{
		if(i%5==0)		//��ʾСʱ�̶���
		{			
			center_xy = Timer_Needle(120,100,60,i,0);
			end_xy = Timer_Needle(120,100,75,i,0);
			LCD_DrawLine((u16)center_xy.end_x, (u16)center_xy.end_y, (u16)end_xy.end_x, (u16)end_xy.end_y);
		}
		else		//��ʾ����̶���
		{
			center_xy = Timer_Needle(120,100,65,i,0);
			end_xy = Timer_Needle(120,100,75,i,0);
			LCD_DrawLine((u16)center_xy.end_x, (u16)center_xy.end_y, (u16)end_xy.end_x, (u16)end_xy.end_y);
		}
	}
	
	POINT_COLOR = RED;
	Timer_Needle(120,100,55,Time.sec,1);		//������
	
	POINT_COLOR = DARKBLUE;
	Timer_Needle(120,100,45,Time.min+((float)Time.sec*(1.0/60.0)),1);		//������
	
	POINT_COLOR = WHITE;
	if(Time.hour>12)  hour_needle=Time.hour-12;		//��ʱ��
	else hour_needle = Time.hour;
	Timer_Needle(120,100,30,(float)hour_needle*5.0+((float)Time.min*(5.0/60.0)),1);
}



End_Coord Timer_Needle(float center_x, float center_y, float length, float time, u8 draw_flog)
{
	End_Coord end_coord;
	
	if((time>=0)&&(time<15))	//��һ����
	{
		end_coord.end_x=center_x + length*cos((90.0-(time*6.0))*(3.1415/180.0));		//����x����
		end_coord.end_y=center_y - length*sin((90.0-(time*6.0))*(3.1415/180.0));		//����y����
	}
	
	else if((time>=15)&&(time<30))	//��������
	{
		end_coord.end_x=center_x + length*cos(((time*6.0)-90.0)*(3.1415/180.0));		//����x����
		end_coord.end_y=center_y + length*sin(((time*6.0)-90.0)*(3.1415/180.0));		//����y����
	}
	
	else if((time>=30)&&(time<45))	//��������
	{
		end_coord.end_x=center_x - length*cos((270.0-(time*6.0))*(3.1415/180.0));		//����x����
		end_coord.end_y=center_y + length*sin((270.0-(time*6.0))*(3.1415/180.0));		//����y����
	}
	
	else if((time>=45)&&(time<60))	//�ڶ�����
	{
		end_coord.end_x=center_x - length*cos(((time*6.0)-270.0)*(3.1415/180.0));		//����x����
		end_coord.end_y=center_y - length*sin(((time*6.0)-270.0)*(3.1415/180.0));		//����y����
	}
	
	if(draw_flog)  LCD_DrawLine((u16)center_x, (u16)center_y, (u16)end_coord.end_x, (u16)end_coord.end_y);	//����ָ��
	
	return end_coord;
}



void Add_Icon(u16 sx,u16 sy)
{
	LCD_Fill(sx,sy,sx+24,sy+24,GRAY);	//ͼ������
	LCD_Fill(sx+4,sy+10,sx+24-4,sy+24-10,WHITE);
	LCD_Fill(sx+10,sy+4,sx+24-10,sy+24-4,WHITE);		
}



void Subtract_Icon(u16 sx,u16 sy)
{
	LCD_Fill(sx,sy,sx+24,sy+24,GRAY);	//ͼ������
	LCD_Fill(sx+4,sy+10,sx+24-4,sy+24-10,WHITE);	
}

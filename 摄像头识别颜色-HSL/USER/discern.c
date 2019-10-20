#include "discern.h"
#include "math.h"
#include "usart.h"


/*RGB565ת����RGB888*/
void RGB565_RGB888(u16 data,COLOR_RGB *Rgb)
{
	Rgb->red   = (unsigned char)((data&0xf800)>>8);
	Rgb->green = (unsigned char)((data&0x07e0)>>3);
	Rgb->blue  = (unsigned char)((data&0x001f));
}


/*RGBת��ΪHSL*/
COLOR_HSL REGtoHSL(COLOR_RGB rgb)
{
    int h,s,l;					//HSL��ɫ����
	int maxVal,minVal,difVal;	//���ֵ����Сֵ����ֵ
	int r = rgb.red;
	int g = rgb.green;
    int b = rgb.blue;
	COLOR_HSL hsl;				//hsl��ɫ�ṹ��
	
	maxVal = max3val(r, g, b);
	minVal = min3val(r, g, b);
	
	difVal = maxVal-minVal;
	
	/*��������*/
    l = (maxVal+minVal)*240/255/2;
	
	if(maxVal == minVal)	//��r=g=b
	{
		h = 0; 
		s = 0;
	}
	else
	{
		/*����ɫ��*/
		if(maxVal==r)
		{
			if(g>=b)
				h = 40*(g-b)/(difVal);
			else
				h = 40*(g-b)/(difVal) + 240;
		}
		else if(maxVal==g)
			h = 40*(b-r)/(difVal) + 80;
		else if(maxVal==b)
			h = 40*(r-g)/(difVal) + 160;
		
		/*���㱥�Ͷ�*/
		if(l == 0)
			s = 0;
		else if(l<=120)
			s = (difVal)*240/(maxVal+minVal);
		else
			s = (difVal)*240/(480 - (maxVal+minVal));
	}
    hsl.hue 	   = (unsigned char)(((h>240)? 240 : ((h<0)?0:h)));
    hsl.saturation = (unsigned char)(((s>240)? 240 : ((s<0)?0:s)));
    hsl.luminance  = (unsigned char)(((l>240)? 240 : ((l<0)?0:l)));
	
	return hsl;
}




/******************************************************************
*��������:	Identify_Color
*��������:	�б����ص����ɫ
*��������:	RGB565���ص�����
*�� �� ֵ:	��
*******************************************************************/
u8 Identify_Color(u16 color)
{
	COLOR_RGB Rgb;	//RGB��ɫ
	COLOR_HSL Hsl;	//HLS��ɫ
	u8 color_flag=0;
	
	/*RGB565ת����RGB888*/
	RGB565_RGB888(color,&Rgb);
	
	/*RGBת��ΪHSL*/
	Hsl = REGtoHSL(Rgb);
	
	printf("H: %d\tS: %d\tL: %d\r\n",Hsl.hue,Hsl.saturation,Hsl.luminance);
	
	/*����HSL�ж���ɫ*/
	if(Hsl.luminance>45 && Hsl.luminance<120)	//���Ǻ�ɫ�Ͱ�ɫ
	{
		if(Hsl.saturation>20)				//���ǻ�ɫ
		{
			if(Hsl.hue<35 || Hsl.hue>220)	//�ж�Ϊ��ɫ
			{
				color_flag=4;
			}
			
			if(Hsl.hue>=35 && Hsl.hue<50)	//�ж�Ϊ��ɫ
			{
				color_flag=5;
			}
			
			if(Hsl.hue>=50 && Hsl.hue<90)	//�ж�Ϊ��ɫ
			{
				color_flag=6;
			}
			
			if(Hsl.hue>=130 && Hsl.hue<180)	//�ж�Ϊ��ɫ
			{
				color_flag=7;
			}
		}
		
		else
		{
			color_flag=3;			//�ж�Ϊ��ɫ
		}
	}
	
	else
	{
		if(Hsl.luminance>=120)		//�ж�Ϊ��ɫ
		{
			color_flag=1;
		}
		else
		{
			color_flag=2;			//�ж�Ϊ��ɫ
		}
	}
	
	return color_flag;
}

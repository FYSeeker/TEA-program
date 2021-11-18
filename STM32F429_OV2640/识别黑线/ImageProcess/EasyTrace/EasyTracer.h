#ifndef EASY_TRACER_H
#define EASY_TRACER_H

#include "sys.h"
#include "ov2640.h"

#define IMG_X 0	  //ͼƬx����
#define IMG_Y 0	  //ͼƬy����
#define IMG_W ImageSize_X //ͼƬ���
#define IMG_H ImageSize_Y //ͼƬ�߶�

#define ALLOW_FAIL_PER 3 //�ݴ��ʣ�û1<<ALLOW_FAIL_PER�����������һ������㣬�ݴ���Խ��Խ����ʶ�𣬵�������Խ��
#define ITERATE_NUM    9 //������������������Խ��ʶ��Խ��ȷ����������Խ��

typedef struct{
    unsigned char  H_MIN;//Ŀ����Сɫ��
    unsigned char  H_MAX;//Ŀ�����ɫ��	
    
	unsigned char  S_MIN;//Ŀ����С���Ͷ�  
    unsigned char  S_MAX;//Ŀ����󱥺Ͷ�
	
	unsigned char  L_MIN;//Ŀ����С����  
    unsigned char  L_MAX;//Ŀ���������
	
	unsigned int  WIDTH_MIN;//Ŀ����С���
	unsigned int  HIGHT_MIN;//Ŀ����С�߶�

	unsigned int  WIDTH_MAX;//Ŀ�������
	unsigned int  HIGHT_MAX;//Ŀ�����߶�

}TARGET_CONDI;//�ж�Ϊ��Ŀ������

typedef struct{
	unsigned int x;//Ŀ���x����
	unsigned int y;//Ŀ���y����
	unsigned int w;//Ŀ��Ŀ��
	unsigned int h;//Ŀ��ĸ߶�
}RESULT;//ʶ����


typedef struct{
    unsigned char  red;             // [0,255]
    unsigned char  green;           // [0,255]
    unsigned char  blue;            // [0,255]
}COLOR_RGB;//RGB��ʽ��ɫ

typedef struct{
    unsigned char hue;              // [0,240]
    unsigned char saturation;       // [0,240]
    unsigned char luminance;        // [0,240]
}COLOR_HSL;//HSL��ʽ��ɫ

typedef struct{
    unsigned int X_Start;              
    unsigned int X_End;
	unsigned int Y_Start;              
    unsigned int Y_End;
}SEARCH_AREA;//����

/*���߱�Ե��Ϣ*/
typedef struct
{
    unsigned int Start_x;              
    unsigned int End_x; 
	unsigned int Center_x; 
}EDGE;


//Ψһ��API���û���ʶ������д��Conditionָ��Ľṹ���У��ú���������Ŀ���x��y����ͳ���
//����1ʶ��ɹ�������1ʶ��ʧ��
int Trace(const TARGET_CONDI *Condition,RESULT *Resu);

void ReadColor(unsigned int x,unsigned int y,COLOR_RGB *Rgb);
void RGBtoHSL(const COLOR_RGB *Rgb, COLOR_HSL *Hsl);

/************************************************************************
�������ܣ�	ʶ����ɫ
��ڲ�����	��
���ز�����	��
************************************************************************/
void ColorDiscern(void);

/************************************************************************
�������ܣ�	ʶ������ɫ
��ڲ�����	x,y:�������
���ز�����	��
************************************************************************/
void PointDiscern(u16 x,u16 y);

/************************************************************************
�������ܣ�	Ѱ������
��ڲ�����	��
���ز�����	��
************************************************************************/
void Find_Midline(void);

#endif

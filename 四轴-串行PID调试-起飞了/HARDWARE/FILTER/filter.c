#include "filter.h"
#include "inv_mpu.h"

/******************
* ��������
* mode==0Ϊ�ڻ������޷�  
* mode==1Ϊ�⻷�����޷�
*******************/
void integral_limit(SITE *variable, char mode)     
{                                                
	SITE inner_I_max = {1000, 1000, 1000},
		 outer_I_max = {1000, 1000, 0};
	if(mode==0)  // �ڻ�
	{
		if( variable->x > inner_I_max.x )
		{
			variable->x = inner_I_max.x;
		}
		if( variable->y > inner_I_max.y )
		{
			variable->y = inner_I_max.y;
		}
		if( variable->z > inner_I_max.z )
		{	
			variable->z = inner_I_max.z;
		}
		if( variable->x < -inner_I_max.x )  
		{
			variable->x = -inner_I_max.x;
		}
		if( variable->y < -inner_I_max.y )
		{
			variable->y = -inner_I_max.y;
		}
		if( variable->z < -inner_I_max.z )
		{
			variable->z = -inner_I_max.z;
		}
	}
	if(mode==1)  // �⻷
	{
		if( variable->x > outer_I_max.x )
		{
			variable->x = outer_I_max.x;
		}
		if( variable->y > outer_I_max.y )
		{
			variable->y = outer_I_max.y;
		}
		if( variable->x < -outer_I_max.x )
		{
			variable->x = -outer_I_max.x;
		}
		if( variable->y < -outer_I_max.y )
		{
			variable->y = -outer_I_max.y;
		}
	}
}

/***************************
* mpu6050���ݻ�ȡ��һ���˲�
* ��ȡ��� return 0��
* ��ȡ���� return ������
****************************/
u8 MPU6050_data_filter(SITE *angle, SITE *gyro)
{
	SITE MpuAngle1, MpuGyro1;
	if(mpu_dmp_get_data(&MpuAngle1, &MpuGyro1)==0)  // ����ŷ���Ǻ�����������
	{
		angle->x = angle->x * 0.7 + MpuAngle1.x * 0.3;
		angle->y = angle->y * 0.7 + MpuAngle1.y * 0.3;
		angle->z = angle->z * 0.7 + MpuAngle1.z * 0.3;

		gyro->x = gyro->x * 0.7 + MpuGyro1.x * 0.3;
		gyro->y = gyro->y * 0.7 + MpuGyro1.y * 0.3;
		gyro->z = gyro->z * 0.7 + MpuGyro1.z * 0.3;
		
		return 0;
	}
	return 1;
}


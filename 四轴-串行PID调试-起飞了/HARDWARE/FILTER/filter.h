#ifndef __FILTER_H
#define __FILTER_H
#include "sys.h"
#include "control.h"


void integral_limit(SITE *variable, char mode);		//��������  
u8 MPU6050_data_filter(SITE *angle, SITE *gyro);	// mpu6050���ݻ�ȡ��һ���˲�


#endif


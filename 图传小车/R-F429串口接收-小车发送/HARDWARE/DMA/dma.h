#ifndef __DMA_H
#define __DMA_H
#include "sys.h"


//DMA����
//mem0addr:�洢����ַ0  ��Ҫ�洢����ͷ���ݵ��ڴ��ַ(Ҳ�����������ַ)
//mem1addr:�洢����ַ1  ��ֻʹ��mem0addr��ʱ��,��ֵ����Ϊ0
//memsize:����Դ��Ŀ�ĵش�������ݳ���
void DMA1Stream5_4_init(u32 mem0addr,u32 mem1addr,u16 memsize);

//USART2_Start,��������
void USART2_Start(void);

//USART2_Stop,�رմ���
void USART2_Stop(void);
	
#endif

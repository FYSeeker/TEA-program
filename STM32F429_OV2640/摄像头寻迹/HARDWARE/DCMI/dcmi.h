#ifndef __DCMI_H
#define __DCMI_H
#include "sys.h"

/*֡��*/
extern u8 ov_frame;

/*֡��ɱ�־*/
extern u8 frame_over;

extern DCMI_HandleTypeDef DCMI_Handler;        //DCMI���
extern DMA_HandleTypeDef  DMADMCI_Handler;     //DMA���

void DCMI_Init(void);
void DCMI_DMA_Init(u32 mem0addr,u32 mem1addr,u16 memsize,u32 memblen,u32 meminc);
void DCMI_Start(void);
void DCMI_Stop(void);
void DCMI_Set_Window(u16 sx,u16 sy,u16 width,u16 height);
void DCMI_CR_Set(u8 pclk,u8 hsync,u8 vsync);
#endif

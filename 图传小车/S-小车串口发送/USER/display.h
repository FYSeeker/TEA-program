#ifndef _display_h_
#define _display_h_

#include "sys.h"

/*����ͷ������С*/
#define OvLcdWide 240
#define OvLcdHigh 320

#define buf_size OvLcdHigh*2*10
/*����buf*/
extern u8* TX_buf;


/*ͼ�����ݴ���*/
void Image_DataHandle(void);

#endif

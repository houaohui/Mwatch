/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief simpleClock watchface 极简时钟
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"


extern void drawVectorChar(int16_t x, int16_t y, unsigned char c,
                            uint16_t color, uint16_t bg, uint8_t size_x,
                            uint8_t size_y);
void setVectorDrawClipWin(int16_t x0, int16_t y0, int16_t x1, int16_t y1);


static display_t draw(void);
void drawSimpleClock(void);





//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
}

static display_t draw()
{
	watchFace_switchUpdate();
	drawSimpleClock();

	return DISPLAY_DONE;
}


void drawSimpleClock(void) {
	byte size_w = 5;
	byte size_h = 7;
#if 1
	setVectorDrawClipWin(0,0,128,64);
	drawVectorChar(3,8,'0'+ div10(timeDate->time.hour),1,1,size_w,size_h);
	drawVectorChar(3+size_w*6,8,'0'+ mod10(timeDate->time.hour),1,1,size_w,size_h);
	
	drawVectorChar(6+64,8,'0'+ div10(timeDate->time.mins),1,1,size_w,size_h);
	drawVectorChar(6+64+size_w*6,8,'0'+ mod10(timeDate->time.mins),1,1,size_w,size_h);
#elif
	drawVectorChar(3,8,'0'+ div10(0),1,1,size_w,size_h);
	drawVectorChar(3+size_w*6,8,'0'+ mod10(0),1,1,size_w,size_h);
	
	drawVectorChar(3+3+64,8,'0'+ div10(0),1,1,size_w,size_h);
	drawVectorChar(3+3+64+size_w*6,8,'0'+ mod10(0),1,1,size_w,size_h);
#endif
	if(millis_get()%1000>500) {
		drawVectorChar(64-3*6/2,-3,'.',1,1,3,3);
		drawVectorChar(64-3*6/2,0+32,'.',1,1,3,3);
	}
}




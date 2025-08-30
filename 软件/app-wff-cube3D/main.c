/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief cube3D watchface 立方体
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"



static display_t draw(void);
extern void drawCude3D(void);
void drawClock(void);



//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
	open_watchface_general(); // 通用的表盘打开函数
}

static display_t draw()
{
	watchFace_switchUpdate();
	drawCude3D();
	drawClock();

	return DISPLAY_BUSY;
}


void drawClock(void) {
	setVectorDrawClipWin(0,0,128,64);
	drawVectorChar(64,2,'0'+ div10(timeDate->time.hour),1,1,4,4);
	drawVectorChar(64+4*8,2,'0'+ mod10(timeDate->time.hour),1,1,4,4);
	
	drawVectorChar(64,2+32,'0'+ div10(timeDate->time.mins),1,1,4,4);
	drawVectorChar(64+4*8,2+32,'0'+ mod10(timeDate->time.mins),1,1,4,4);
}



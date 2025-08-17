/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief TerminalSim watchface 模拟终端
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"



static uint32_t seed = 1; // ̦ܺ˽זؓ

static void randomSeed(uint32_t newSeed) {
    seed = newSeed;
}

static long random(long max) {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF; // ПєͬԠʺԉǷ٫ʽ
    return seed % max; // ȡģӔО׆׶Χ
}

static long random_range(long min, long max) {
    return min + random(max - min); // ʹԃʏĦք̦ܺ˽ʺԉǷ
}


static display_t draw(void);

display_t terminalSim_draw(void);
void terminalSim_setup(void);

//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
	terminalSim_setup();
}

static display_t draw()
{
	// Draw time animated
	display_t busy;
	watchFace_switchUpdate();
	
	busy = ticker(0, 0, 1);
	busy = terminalSim_draw();

	return busy;
}

static char m[1][32]; // 16 chars across, 8 chars high
static int ySpeeds = 50;
void terminalSim_setup(void)
{
	randomSeed(millis_get());
	// fill 2D array with random characters
	for(int i=0; i<COUNT_OF(m); i++){
		for(int j=0; j<COUNT_OF(m[i]); j++){
			m[i][j] = random_range(1,64);
		}
	}
}


display_t terminalSim_draw(void)
{

	for(int j=0; j<COUNT_OF(m[0]); j++){
 
		// move the cursor to the right position with good spacing between chars
		// draw the char partially off the screen for a smooth effect
		byte x = 0;
		byte y = (j*2+64-millis_get()/ySpeeds)%(64);

		if(y >= 64){
			m[0][j] = random(64);  // randomly change the character once in a while
		}
		
		LOOP(m[0][j], i)
			draw_point(x+i*2, y, 1, 0);
	}
	
	return DISPLAY_BUSY;
}

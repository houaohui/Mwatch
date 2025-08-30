/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief codeRain watchface 黑客帝国
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"

#define COUNT_OF(array) (sizeof(array) / sizeof(array[0]))


static uint32_t seed = 1; // ̦ܺ˽זؓ

void randomSeed(uint32_t newSeed) {
    seed = newSeed;
}

long random(long max) {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF; // ПєͬԠʺԉǷ٫ʽ
    return seed % max; // ȡģӔО׆׶Χ
}

long random_range(long min, long max) {
    return min + random(max - min); // ʹԃʏĦք̦ܺ˽ʺԉǷ
}


static display_t draw(void);

void codeRain_setup(void);
display_t codeRain_draw(void);

//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
	open_watchface_general(); // 通用的表盘打开函数
	codeRain_setup();
}

static display_t draw()
{
	// Draw time animated
	display_t busy;
	watchFace_switchUpdate();
	
	busy = ticker(0, 0, 1);
	busy = codeRain_draw();

	return busy;
}

char m[16][8]; // 16 chars across, 8 chars high
int ySpeeds[16];

void codeRain_setup(void)
{
	randomSeed(millis_get());
	// fill 2D array with random characters
	for(int i=0; i<COUNT_OF(m); i++){
		for(int j=0; j<COUNT_OF(m[i]); j++){
			m[i][j] = random(95) + + 0x20;
		}
	}
 
	// randomize the speed that columns move
	for(int i=0; i<COUNT_OF(ySpeeds); i++){
		ySpeeds[i] = random_range(8,50);
	}
}


display_t codeRain_draw(void)
{
	for(int i=0; i<COUNT_OF(m); i++){
		for(int j=0; j<COUNT_OF(m[i]); j++){
	 
			// move the cursor to the right position with good spacing between chars
			// draw the char partially off the screen for a smooth effect
			byte x = i*8;
			int y = (j*8+64+millis_get()/ySpeeds[i])%(128+ySpeeds[i]%48)-8;

			if(random(1000) > 900){
				m[i][j] = random(95) + 0x20;  // randomly change the character once in a while
			}
			
			// print character in 2D array
			char c = m[i][j] - 0x20;
			if(y < 64 && y > -8)
			draw_bitmap(x, y, (const unsigned char *)smallFont[(byte)c], SMALLFONT_WIDTH, SMALLFONT_HEIGHT, 0, 0);
		}
	}
	return DISPLAY_BUSY;
}

/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief snow watchface 雪
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


static void snowSim_init(void);
//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
	snowSim_init();
	openLSM6DSM(LSM_ACC_GYRO);
}


static void solver_update(void);

// sand sim
#define MAX_SNOWS   (200)
typedef struct {
	byte actived;
	byte x;
	byte y;
} snow_s;

typedef struct {
	millis_t time_counter;
	byte activedCount;
	snow_s sands[MAX_SNOWS];
	int8_t acc_x;
	int8_t acc_y;
} SolverSnow_s;

SolverSnow_s solver_snow;

static display_t draw()
{
	watchFace_switchUpdate();

	ticker(0, 0, 1);
	solver_update();
	if(LSM6DSM->acc_y > 0.2) {
		solver_snow.acc_x = -1;
	} else if(LSM6DSM->acc_y < -0.2) {
		solver_snow.acc_x = 1;
	} else {
		solver_snow.acc_x = 0;
	}

	return DISPLAY_BUSY;
}









static void snowSim_init(void) {
	memset(solver_snow.sands, 0, MAX_SNOWS*sizeof(snow_s));
	solver_snow.time_counter = 0;
	solver_snow.activedCount = 0;
	solver_snow.acc_x = 0;
	solver_snow.acc_y = 1;
}

static int getPixel(int16_t x, int16_t y) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        return (oledBuffer[x + (y / 8) * 128] & (1 << (y % 8))) != 0;
    }
    return 0; // ӬԶ׶Χ
}



static bool gen_snows(byte count) {
	static byte last_x,last_y;
	static byte now_seed_count = 0;

	int index = 0;  // find next
	while(solver_snow.sands[index].actived) {
		if(++index >= MAX_SNOWS) break;
	}
	
	solver_snow.sands[index].actived = 250;
	// ̦ܺλ׃ۍ̙׈
	randomSeed(millis_get());
	solver_snow.sands[index].x = random_range(0, 127);
	solver_snow.sands[index].y = 255;
	
	if(last_x == solver_snow.sands[index].x && last_y == solver_snow.sands[index].y)
		return false;
	last_x = solver_snow.sands[index].x;
	last_y = solver_snow.sands[index].y;
	now_seed_count++;
	if(now_seed_count < count)
		return false;
	if(now_seed_count == count) {
		now_seed_count = 0;
		return true;
	}
	
	return false;
}

static void snow_move(snow_s *sand) {
	if(sand->y == 63)
		return;
	// collide
	if(getPixel(sand->x, sand->y)) {
		while(getPixel(sand->x + solver_snow.acc_x, sand->y+ solver_snow.acc_y)) {
			sand->x += solver_snow.acc_x;
			sand->y += solver_snow.acc_y;
			if(sand->x == 255 || sand->x == 128)
				sand->actived = false;
			if(sand->y == 255 || sand->y == 64)
				sand->actived = false;
		}
	}
	
	byte x_flag = 0, y_flag = 0;
	// move accx
	if(getPixel(sand->x + solver_snow.acc_x, sand->y) == 0)
	{
		x_flag = 1;
	}
	
	// move accy
	if(getPixel(sand->x , sand->y + solver_snow.acc_y) == 0)
	{
		y_flag = 1;
	}
	
	if(x_flag && y_flag)
	{
		if(getPixel(sand->x + solver_snow.acc_x, sand->y+ solver_snow.acc_y))
			x_flag = 0;
	}
	
	
	if(y_flag) { // äЂһӆ֯
		sand->y += solver_snow.acc_y;
		if(x_flag)
			sand->x += solver_snow.acc_x;
	}
	
	if(sand->y == 64)
		sand->y = 63;

}


static void solver_update(void) {
	static byte start_seed_flag = 0;
	
	int one_time_count = 10;
	if(solver_snow.time_counter%30 == 0) {
		if(solver_snow.activedCount < MAX_SNOWS - one_time_count) {
			start_seed_flag = 1;
		}
	}
	
	if(start_seed_flag) {
		if(gen_snows(one_time_count))
			start_seed_flag = false;
	}
	
	solver_snow.activedCount = 0;
	
	for(int i = 0; i < MAX_SNOWS; i++) {
		
		if(solver_snow.sands[i].actived) {
			
			solver_snow.activedCount++;
			// ϯmӽȦ
			// update position
			if(solver_snow.time_counter%(2) == 0) {
				solver_snow.sands[i].actived--;
				snow_move(&solver_snow.sands[i]);
			}
			
			
			// draw ball
			draw_point(solver_snow.sands[i].x, solver_snow.sands[i].y, 1, 0);
			
		}
		
	}
	
	solver_snow.time_counter++;

}

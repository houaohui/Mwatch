/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief sandBox watchface 沙盒
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


void sandSim_init(void);
//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
	sandSim_init();
	openLSM6DSM(LSM_ACC_GYRO);
}


void solver_update(void);
// sand sim
#define MAX_SANDS   (250)
typedef struct {
	byte actived;
	byte x;
	byte y;
} sand_s;

typedef struct {
	millis_t time_counter;
	byte activedCount;
	sand_s sands[MAX_SANDS];
	int8_t acc_x;
	int8_t acc_y;
} Solver_s;

Solver_s solver;

#define SHOW_FPS 1

static display_t draw()
{
#if SHOW_FPS
	static millis8_t lastUpdate;
	millis8_t delt_ms = millis_get() - lastUpdate;
	lastUpdate = millis_get();
	float fps = 1000/(float)delt_ms;
	char buff[10];
	sprintf_P(buff, "%.2f", fps);
	draw_string(buff,0,0,0);
#endif
	watchFace_switchUpdate();
	
	if(LSM6DSM->acc_y > 0.2) {
		solver.acc_x = -1;
	} else if(LSM6DSM->acc_y < -0.2) {
		solver.acc_x = 1;
	} else {
		solver.acc_x = 0;
	}
	
	if(LSM6DSM->acc_x > 0.2) {
		solver.acc_y = -1;
	} else if(LSM6DSM->acc_x < -0.2) {
		solver.acc_y = 1;
	} else {
		solver.acc_y = 0;
	}
	
	
	ticker(0, 0, 1);
	solver_update();

	return DISPLAY_BUSY;
}









void sandSim_init(void) {
	memset(solver.sands, 0, MAX_SANDS*sizeof(sand_s));
	solver.time_counter = 0;
	solver.activedCount = 0;
	solver.acc_x = 0;
	solver.acc_y = 1;
}

static int getPixel(int16_t x, int16_t y) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        return (oledBuffer[x + (y / 8) * 128] & (1 << (y % 8))) != 0;
    }
    return 0; // ӬԶ׶Χ
}



static bool gen_sands(byte count) {
	static byte last_x,last_y;
	static byte now_seed_count = 0;

	int index = 0;  // find next
	while(solver.sands[index].actived) {
		if(++index >= MAX_SANDS) break;
	}
	
	solver.sands[index].actived = 250;
	// ̦ܺλ׃ۍ̙׈
	randomSeed(millis_get());
	solver.sands[index].x = random_range(0, 127);
	solver.sands[index].y = 64-8;
	
	if(last_x == solver.sands[index].x && last_y == solver.sands[index].y)
		return false;
	last_x = solver.sands[index].x;
	last_y = solver.sands[index].y;
	now_seed_count++;
	if(now_seed_count < count)
		return false;
	if(now_seed_count == count) {
		now_seed_count = 0;
		return true;
	}
	
	return false;
}

static bool is_collode_sand(byte x, byte y) {
	for(int i=0; i< MAX_SANDS; i++) {
		if(solver.sands[i].actived 
			&& x == solver.sands[i].x && y == solver.sands[i].y)
			return true;
	}
	return false;
}

void sand_move(sand_s *sand) {
	if(!solver.acc_x && !solver.acc_y)
		return;
	// collide
	if(getPixel(sand->x, sand->y)) {
		while(getPixel(sand->x + solver.acc_x, sand->y+ solver.acc_y)) {
			sand->x += solver.acc_x;
			sand->y += solver.acc_y;
			if(sand->x == 255 || sand->x == 128)
				sand->actived = false;
			if(sand->y == 255 || sand->y == 64)
				sand->actived = false;
		}
	}
	
	byte x_flag = 0, y_flag = 0;
	// move accx  ûԐб̘֣Ȓsand ۍsand ֮ݤûԐƶײ
	if(getPixel(sand->x + solver.acc_x, sand->y) == 0 && !is_collode_sand(sand->x + solver.acc_x, sand->y))
	{
		x_flag = 1;
	}
	
	// move accy
	if(getPixel(sand->x , sand->y + solver.acc_y) == 0 && !is_collode_sand(sand->x, sand->y + solver.acc_y))
	{
		y_flag = 1;
	}
	
	if(x_flag && y_flag)
	{
		if(getPixel(sand->x + solver.acc_x, sand->y+ solver.acc_y) || is_collode_sand(sand->x + solver.acc_x, sand->y + solver.acc_y))
			y_flag = 0;
	}
	
	// xࠉӆ֯
	if(x_flag) {
		sand->x += solver.acc_x;
	}
	
	// y ࠉӆ֯
	if(y_flag) {
		sand->y += solver.acc_y;
	}

	if(sand->x == 128)
		sand->x = 127;
	
	if(sand->x == 255)
		sand->x = 0;
	
	if(sand->y == 64)
		sand->y = 63;
	
	if(sand->y == 255)
		sand->y = 0;
}


void solver_update(void) {
	static byte start_seed_flag = 0;
	
	int one_time_count = 10;
	if(solver.time_counter%10 == 0) {
		if(solver.activedCount < MAX_SANDS - one_time_count) {
			start_seed_flag = 1;
		}
	}
	
	if(start_seed_flag) {
		if(gen_sands(one_time_count))
			start_seed_flag = false;
	}
	
	solver.activedCount = 0;
	
	for(int i = 0; i < MAX_SANDS; i++) {
		
		if(solver.sands[i].actived) {
			
			solver.activedCount++;
			// ϯmӽȦ
			// update position
			if(solver.time_counter%(2) == 0) {
//				solver.sands[i].actived--;
				sand_move(&solver.sands[i]);
			}
			
			// draw ball
			draw_point(solver.sands[i].x, solver.sands[i].y, 1, 0);
			
			
		}
		
	}
	
//	char buff[5];
//	
//	sprintf_P(buff, PSTR("%d"), solver.activedCount );
//	draw_string(buff,false,0,64-16 -8);
//	
//	sprintf_P(buff, PSTR("%d"), solver.acc_x);
//	draw_string(buff,false,0,64-16);
//	
//	sprintf_P(buff, PSTR("%d"), solver.acc_y);
//	draw_string(buff,false,0,64-8);
	
	solver.time_counter++;

}


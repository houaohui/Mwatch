/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief galaxy watchface 星系
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"



static display_t draw(void);


extern void galaxy_draw(void);
void random_drawStars(void);

//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
	openLSM6DSM(LSM_ACC_GYRO);
}
 
static float x_gyro = 0, y_gyro = 0;

static display_t draw()
{
	watchFace_switchUpdate();
	

	do {
		x_gyro += LSM6DSM->gyr_x * 0.01;
		y_gyro += -LSM6DSM->gyr_y * 0.01;
		if(x_gyro > 10)
			x_gyro = 10;
		if(x_gyro < -10)
			x_gyro = -10;
		
		if(y_gyro > 10)
			y_gyro = 10;
		if(y_gyro < -10)
			y_gyro = -10;
	} while(0);
	
	galaxy_draw();
	random_drawStars();
	
	ticker(x_gyro,y_gyro, 0);
	return DISPLAY_BUSY;
}

#include "math.h"
static float radians(float angle) {
	return angle * 0.01745;
}


void draw_spiral_stars(u16 num_stars, u16 arm_length, u16 spread, u16 rotation_offset) {
	LOOP(num_stars, i) {
		int angle, length, x, y;
		// ݆̣އ׈ۍӤ׈ì̹ԐқʹԃРͬք spread
		angle = i * spread + rotation_offset;  //  ͭݓһٶѽתƫӆ
        length = arm_length * i / num_stars;
		
		// ݆̣ÝѽПքĩ׋ظҪ
        x = length * cos(radians(angle)) *3/5;
        y = length * sin(radians(angle)) *3/5;
//		x *= cos(x_gyro*9*0.01745);
		y *= cos(y_gyro*9*0.01745);
		
//		draw_point(x + 64 + (int)x_gyro, y + 32 + (int)y_gyro, 1, 0);
		draw_point(x + 64, y + 32, 1, 0);
	}
}


typedef struct {
	byte x;
	byte y;
	byte time;
} start_s;

static start_s starts[50];

void random_drawStars(void) {
	static  uint32_t time_counter = 0;
	if(time_counter%2 == 0) {
		int i = 0;  // find next circle
		while(starts[i].time) {
			if(++i >= 50) break;
		}
		if(i < 50) {
			// Ҫ݇Ҩφʺԉ
			starts[i].time = 30;
			starts[i].x = rand()%128;
			starts[i].y = rand()%64;
		}
	}
	
	LOOP(50, i) {
		if(starts[i].time) {
			starts[i].time--;
			
			draw_point(starts[i].x, starts[i].y, 1, 0);
		}
		
	}
	time_counter++;
}

// 
void galaxy_draw(void) {
	static u16 rotation_angle = 0;
	LOOP(4, i)
		draw_spiral_stars(15, 130, 10, i * 90 + rotation_angle); // 画旋臂

	if(++rotation_angle == 360)
		rotation_angle = 0;
}


/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief neno watchface neno
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"





static display_t draw(void);
void neno_draw(void);


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
		if(x_gyro > 20)
			x_gyro = 20;
		if(x_gyro < -20)
			x_gyro = -20;
		
		if(y_gyro > 20)
			y_gyro = 20;
		if(y_gyro < -20)
			y_gyro = -20;
	} while(0);
	
	neno_draw();
	ticker(x_gyro,y_gyro, false);
	return DISPLAY_BUSY;
}


#ifndef _swap_char
#define _swap_char(a, b)     \
    {                        \
        s16 t = a; \
        a = b;               \
        b = t;               \
    }
#endif
static void myOLED_DrawLine_(int16_t x0, int16_t y0, int16_t x1, int16_t y1, u8 t, bool absolute)
{
    s16 dx, dy, ystep;
    int err;
    u8 swapxy = 0;

    dx = abs(x1 - x0);//ȳ߸הֵ
    dy = abs(y1 - y0);

    if (dy > dx)
    {
        swapxy = 1;
        _swap_char(dx, dy);
        _swap_char(x0, y0);
        _swap_char(x1, y1);
    }

    if (x0 > x1)
    {
        _swap_char(x0, x1);
        _swap_char(y0, y1);
    }
    err = dx >> 1;

    if (y0 < y1)
    {
        ystep = 1;
    }
    else
    {
        ystep = -1;
    }

    for (; x0 <= x1; x0++)
    {
        if (swapxy == 0)
            draw_point(x0, y0,t,absolute);
        else
            draw_point(y0, x0,t,absolute);
        err -= dy;
        if (err < 0)
        {
            y0 += ystep;
            err += dx;
        }
    }
}


#define TRIANGLE_COUNT  (10)
typedef struct {
	byte x;
	byte y;
	byte len;
	byte active;
	byte speed;
} triangle_s;

typedef struct  {
	int center_x;
	int center_y;
	uint32_t time_counter;  // ԃԚ݆ʱʺԉˮҨքʱݤݤٴ
	triangle_s  triangles[TRIANGLE_COUNT];
} neno_s;

neno_s neno = {0};

void neno_init(void) {
	
	memset(&neno, 0, sizeof(neno_s));
}

void neno_draw(void) {
	
	if(neno.time_counter%50 == 0) {
		int i = 0;  // find next circle
		while(neno.triangles[i].active) {
			if(++i >= TRIANGLE_COUNT) break;
		}
		if(i < TRIANGLE_COUNT) {
			// Ҫ݇Ҩφʺԉ
			neno.triangles[i].active = 150;
			neno.triangles[i].len = 10;
			neno.triangles[i].x = 64;
			neno.triangles[i].y = 40;
			neno.triangles[i].speed  = 12;
		}
	}
	
	int active_cnt = 0;
	for (int i=0; i<TRIANGLE_COUNT; i++) {
		if(neno.triangles[i].active) {
			active_cnt++;
//			int speed = 8*neno.triangles[i].active/120;
			if(neno.time_counter%20 == 0) {
				if(neno.triangles[i].speed > 1)
					neno.triangles[i].speed--;
			}

			if(neno.time_counter%(neno.triangles[i].speed) == 0) {
				neno.triangles[i].len ++;
			}
			
			if(neno.time_counter%2 == 0) {
				neno.triangles[i].active--;
			}
			// x1, y1
			int x1 = neno.triangles[i].x - neno.triangles[i].len;
			int y1 = neno.triangles[i].y + neno.triangles[i].len/2;
			
			
			int x2 = neno.triangles[i].x;
			int y2 = neno.triangles[i].y - neno.triangles[i].len;
			
			int x3 = neno.triangles[i].x + neno.triangles[i].len;
			int y3 = neno.triangles[i].y + neno.triangles[i].len/2;
			int offset_x =  (int)x_gyro*(neno.triangles[i].len)/30;
			int offset_y =  (int)y_gyro*(neno.triangles[i].len)/30;
			myOLED_DrawLine_(x1 + offset_x, y1+offset_y, x2+offset_x, y2+offset_y, 1, 0);
			myOLED_DrawLine_(x2 + offset_x, y2+offset_y, x3+offset_x, y3+offset_y, 1, 0);
		}
	}
//	char buff[10];
//	sprintf_P(buff, "%d", active_cnt);
//	draw_string(buff,0,0,0);
	neno.time_counter++;

}







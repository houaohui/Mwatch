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
#define draw_point_(a, b, c)  draw_point(a, b, c, false)
static void drawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color) {
	int16_t f = 1 - r;
	int16_t ddF_x = 1;
	int16_t ddF_y = -2 * r;
	int16_t x = 0;
	int16_t y = r;
	
	draw_point_(x0, y0 + r, color);
	draw_point_(x0, y0 - r, color);
	draw_point_(x0 + r, y0, color);
	draw_point_(x0 - r, y0, color);

	while (x < y) {
		if (f >= 0) {
			y--;
			ddF_y += 2;
			f += ddF_y;
		}
		x++;
		ddF_x += 2;
		f += ddF_x;
			
		draw_point_(x0 + x, y0 + y, color);
		draw_point_(x0 - x, y0 + y, color);
		draw_point_(x0 + x, y0 - y, color);
		draw_point_(x0 - x, y0 - y, color);

		draw_point_(x0 + y, y0 + x, color);
		draw_point_(x0 - y, y0 + x, color);
		draw_point_(x0 + y, y0 - x, color);	
		draw_point_(x0 - y, y0 - x, color);			
	}
}

static display_t draw(void);



//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
}

void draw_wavesSim(void);
static display_t draw()
{
	watchFace_switchUpdate();
	draw_wavesSim();
	ticker(0, 0, 1);

	return DISPLAY_DONE;
}

// ģŢˮҨ
#define  MAX_WAVES   3   // ͬʱզ՚քخճˮҨ˽
#define  MAX_WAVE_SPEED   5
#define  MAX_WAVE_CIRCLES  5    // ÿٶˮҨخճքҨφ
#define  WAVELENGTH  5  // ҨӤ
typedef struct {
	byte actived;
	byte radius;
	byte active_time;  // եλī
	byte pos_x;
	byte pos_y;
} wave_radius_s;

typedef struct  {
	int center_x;
	int center_y;
	int8_t speed_x;
	int8_t speed_y;
	byte strength;  // ǿ׈ ʺԉخճքҨφìۍԖѸʱӤ
	byte active;  // ˇرֽ՚ܮԾ
	uint32_t time_counter;  // ԃԚ݆ʱʺԉˮҨքʱݤݤٴ
	wave_radius_s  waves_radius[8];
} wave_s;

wave_s waves_arry[MAX_WAVES] = {0};


void single_wave_update(wave_s * wave) {
	if(wave->active) {
		
		if(wave->time_counter%25 == 0) {
			int i = 0;  // find next circle
			while(wave->waves_radius[i].actived) {
				if(++i >= 8) break;
			}
			if(i < wave->strength) {
				// Ҫ݇Ҩφʺԉ
				wave->waves_radius[i].actived = true;
				// եٶҨφԖѸʱݤ
				wave->waves_radius[i].active_time = wave->strength*3/2;
			}
		}
		
		int circle_active_count = 0;
		// ټтˮҨѫ޶ ۍ ˮҨԖѸʱݤ
		for (int i=0; i<wave->strength; i++) {
			if(wave->waves_radius[i].active_time&&wave->waves_radius[i].actived) {
				circle_active_count++;
				// pos update
				wave->waves_radius[i].pos_x =wave->center_x + i*wave->speed_x;
				wave->waves_radius[i].pos_y =wave->center_y + i*wave->speed_y;
				
				
				if(wave->time_counter%2 == 0)
					wave->waves_radius[i].radius++;
				if(wave->time_counter%10 == 0)
					wave->waves_radius[i].active_time--;
				// ۭˮҨ ҩz̹ԐքˮҨ
				drawCircle(wave->waves_radius[i].pos_x, wave->waves_radius[i].pos_y, wave->waves_radius[i].radius, 1);
			}
		}
		
		if(circle_active_count == 0 && wave->waves_radius[0].actived) {
			wave->active = false;
//			// reset
//			memset(wave->waves_radius, 8, sizeof(wave_radius_s));
		}
		
		wave->time_counter++;
	
	}
}




void draw_wavesSim(void) {
	  static millis8_t lastUpdate;

	millis8_t now = millis_get();

	
	// ̦ܺʺԉظҪìǿ׈ìҢͭݓս˽ةא
	if((millis8_t)(now - lastUpdate) >= 5555) {
		randomSeed(millis_get());
		int i = 0;  // find next circle
		while(waves_arry[i].active) {
			if(++i >= 8) break;
		}
		if(i < MAX_WAVES) {
			memset(&waves_arry[i], 0, sizeof(wave_s));
			waves_arry[i].active = true;
			waves_arry[i].strength = random_range(6, 8);
			waves_arry[i].center_x = random_range(16, 128-16);
			waves_arry[i].center_y = random_range(16, 64-16);
			waves_arry[i].speed_x = random_range(-MAX_WAVE_SPEED, MAX_WAVE_SPEED);
			waves_arry[i].speed_y = random_range(-MAX_WAVE_SPEED, MAX_WAVE_SPEED);
		}
		lastUpdate = now;
	}
	
	
	for (int i=0; i<MAX_WAVES; i++) {
		single_wave_update(&waves_arry[i]);
	}
}

/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief simClock watchface 模拟时钟
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"



static display_t draw(void);

void drawSimClock(void);


//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
		open_watchface_general(); // 通用的表盘打开函数
}

static display_t draw()
{
	watchFace_switchUpdate();
	drawSimClock();
	// busy = ticker();

	return DISPLAY_BUSY;
}



#include "math.h"
static float radians(float angle) {
	return angle * 0.01745;
}

static byte getNumWidth(int num){
	if(num>=100)
		return 15;
	else if(num >= 10)
		return 10;
	else
		return 5;
} 

void drawSimClock(void) 
{

	signed char precalculated_x_radius_pixel; // lookup table to prevent expensive sin/cos calculations
	signed char precalculated_y_radius_pixel; // lookup table to prevent expensive sin/cos calculations

	int center_x = 64;
	int center_y = 32;
	int radius_pixel = 14 + 17;

	signed char start_x = 0;
	signed char start_y = 0;

	float angle = 0;
	

	for (int i = 0; i <= 360; i=i+15) {
		angle = i; // potentiometer_value is multiplied by 10, 3£ = 1 step     

		precalculated_x_radius_pixel = round(sin(radians(angle))*radius_pixel + center_x);
		precalculated_y_radius_pixel = round(-cos(radians(angle))*radius_pixel + center_y);
		if (precalculated_x_radius_pixel < 0 ||  precalculated_y_radius_pixel > 64
			|| precalculated_x_radius_pixel >= 127 || precalculated_y_radius_pixel < 0)
			continue;

		// get values from pre-calculated look-up table
		start_x = precalculated_x_radius_pixel;
		start_y = precalculated_y_radius_pixel;


		if (start_x > 0 && start_y > 0 && start_x < 127 && start_y < 64) { 
			
			if (i % 90 == 0) {
				int num;
				if(i == 0)
					num = 12;
				else
					num = i/30;
				if(num == 12) {
					char c = '1' - 0x20;
					draw_bitmap(start_x-getNumWidth(num)/2, start_y, (const unsigned char *)smallFont[(byte)c], SMALLFONT_WIDTH, SMALLFONT_HEIGHT, 0, 0);
					c = '2' - 0x20;
					draw_bitmap(start_x+7-2-getNumWidth(num)/2, start_y, (const unsigned char *)smallFont[(byte)c], SMALLFONT_WIDTH, SMALLFONT_HEIGHT, 0, 0);
				} else {
					char buff[5];
					if(num == 3|| num == 9) {
						start_y -= 3;
					} else if(num == 6) {
						start_y -= 6;
					}
					sprintf_P(buff, PSTR("%d"), num);
					draw_string(buff, false, start_x-getNumWidth(num)/2, start_y);
				}
				
				continue;
			}
			if (i % 15 == 0) { // this is a big tickmark, as it¤s divisible by 10

				draw_point(start_x, start_y,1,0); // dots instead of lines  
			}

		}
	}
	
	// 
	
	
	// draw time

	static int now_secs_angle = 0;
	static bool is_skip_60_to_0 = false;
	int target_secs_angle = timeDate->time.secs * 6;
	int speed;
	// ҹ֡
	if(target_secs_angle == 0) {
		target_secs_angle = 360; 
		is_skip_60_to_0 = true;
	}
	if(is_skip_60_to_0 && target_secs_angle == 6) {
		is_skip_60_to_0 = false;
		now_secs_angle = 0;
	}
	if(now_secs_angle!=target_secs_angle) {
		speed = ((target_secs_angle - now_secs_angle) > 0) ? ((target_secs_angle - now_secs_angle)/2 + 1) : ((target_secs_angle - now_secs_angle)/2 - 1);
		speed = abs(speed)> 1 ? (speed > 0 ? 1 : -1) : speed;
		now_secs_angle += speed;
	}
	
	// draw secs point
	int radius_secs_point = 14 + 17 - 7;
	signed char x_radius_secs_point = round(sin(radians(now_secs_angle))*radius_secs_point + center_x);
	signed char y_radius_secs_point = round(-cos(radians(now_secs_angle))*radius_secs_point + center_y);

	myOLED_DrawLine(center_x, center_y, x_radius_secs_point, y_radius_secs_point, 1, 0);
	
	// draw mins point
	int radius_mins_point = 14 + 17 - 7 -7;
	int target_mins_angle = timeDate->time.mins*6 + (timeDate->time.secs*6)/60;
	signed char x_radius_mins_point = round(sin(radians(target_mins_angle))*radius_mins_point + center_x);
	signed char y_radius_mins_point = round(-cos(radians(target_mins_angle))*radius_mins_point + center_y);
	signed char x_radius_mins_point_l = round(sin(radians(target_mins_angle-90))*2 + center_x);
	signed char y_radius_mins_point_l = round(-cos(radians(target_mins_angle-90))*2 + center_y);
	signed char x_radius_mins_point_r = round(sin(radians(target_mins_angle+90))*2 + center_x);
	signed char y_radius_mins_point_r = round(-cos(radians(target_mins_angle+90))*2 + center_y);

	myOLED_DrawLine(x_radius_mins_point_l, y_radius_mins_point_l, x_radius_mins_point, y_radius_mins_point, 1, 0);
		myOLED_DrawLine(x_radius_mins_point_r, y_radius_mins_point_r, x_radius_mins_point, y_radius_mins_point, 1, 0);
	
	// draw mins point
	int radius_hour_point = 14 + 17 - 7 - 7 - 7;
	int target_hour_angle = timeDate->time.hour*30 + target_mins_angle/60;
	signed char x_radius_hour_point = round(sin(radians(target_hour_angle))*radius_hour_point + center_x);
	signed char y_radius_hour_point = round(-cos(radians(target_hour_angle))*radius_hour_point + center_y);
	signed char x_radius_hour_point_l = round(sin(radians(target_hour_angle-90))*2 + center_x);
	signed char y_radius_hour_point_l = round(-cos(radians(target_hour_angle-90))*2 + center_y);
	signed char x_radius_hour_point_r = round(sin(radians(target_hour_angle+90))*2 + center_x);
	signed char y_radius_hour_point_r = round(-cos(radians(target_hour_angle+90))*2 + center_y);

	myOLED_DrawLine(x_radius_hour_point_l, y_radius_hour_point_l, x_radius_hour_point, y_radius_hour_point, 1, 0);
		myOLED_DrawLine(x_radius_hour_point_r, y_radius_hour_point_r, x_radius_hour_point, y_radius_hour_point, 1, 0);
	
	
	// draw circle
	draw_point(center_x-1, center_y-2, 1, 0);
	draw_point(center_x, center_y-2, 1, 0);
	draw_point(center_x+1, center_y-2, 1, 0);
	
	draw_point(center_x-2, center_y-1, 1, 0);
	draw_point(center_x-2, center_y, 1, 0);
	draw_point(center_x-2, center_y+1, 1, 0);
	
	draw_point(center_x-1, center_y+2, 1, 0);
	draw_point(center_x, center_y+2, 1, 0);
	draw_point(center_x+1, center_y+2, 1, 0);
	
	draw_point(center_x+2, center_y-1, 1, 0);
	draw_point(center_x+2, center_y, 1, 0);
	draw_point(center_x+2, center_y+1, 1, 0);
}

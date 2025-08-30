/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief balls watchface 弹球
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// ����ϵͳ����API
#include "watchface_api.h"




static uint32_t seed = 1; // ?????

static void randomSeed(uint32_t newSeed) {
    seed = newSeed;
}

static long random(long max) {
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF; // ��????????
    return seed % max; // ???��??��
}

static long random_range(long min, long max) {
    return min + random(max - min); // ???????????
}




static display_t draw(void);


void balls_init(void);
//watch face
void my_main(void)
{
	display_setDrawFunc(draw);
	open_watchface_general(); // ͨ�õı��̴򿪺���
	balls_init();
}


void ball_balls(void);

static display_t draw()
{
	watchFace_switchUpdate();

	ticker(0, 0, 1);
	ball_balls();

	return DISPLAY_BUSY;
}

//  ball ball

typedef enum {
	NO_C = 0,
	UD_C, // ??????
	LR_C,
	A_C,
	C_C,
} Collide_t;

typedef struct {
	byte actived;
	byte x;
	byte y;
	int8_t speed_x;
	int8_t speed_y;
} ball_s;

#define MAX_BALLS   (100)
#define BALL_MAX_SPEED  (1)

static millis_t time_counter = 0;
static byte activedCount = 0;
ball_s balls_arry[MAX_BALLS];

void balls_init(void) {
	memset(balls_arry, 0, MAX_BALLS*sizeof(ball_s));
	time_counter = 0;
}

static int getPixel(int16_t x, int16_t y) {
    if (x >= 0 && x < 128 && y >= 0 && y < 64) {
        return (oledBuffer[x + (y / 8) * 128] & (1 << (y % 8))) != 0;
    }
    return 0; // ???��
}

Collide_t collide(ball_s* ball) {
	if(getPixel(ball->x, ball->y))
		return C_C;

	Collide_t lr_flag = NO_C, ud_flag = NO_C;
	if(getPixel(ball->x, ball->y + ball->speed_y) ||ball->y ==0 || ball->y == 63) {
		ud_flag = UD_C;
	} else if(getPixel(ball->x + ball->speed_x, ball->y)||ball->x ==0|| ball->x ==127) {
		lr_flag = LR_C;
	}
	if(lr_flag||ud_flag) {
		if(lr_flag && ud_flag)
			return A_C;
		else
			return ud_flag ? ud_flag : lr_flag;
	}
	
	if(getPixel(ball->x + ball->speed_x, ball->y + ball->speed_y))
	{
		return A_C;
	}
	
	return NO_C;
}

static bool seed_ball(byte count, byte x, byte y, int8_t speed_x, int8_t speed_y) {
	static byte now_seed_count = 0;

	int index = 0;  // find next
	while(balls_arry[index].actived) {
		if(++index >= MAX_BALLS) break;
	}
	
	balls_arry[index].actived = true;
	// ??��????
	balls_arry[index].x = x;
	balls_arry[index].y = y;
	balls_arry[index].speed_x = speed_x;
	balls_arry[index].speed_y = speed_y;
	now_seed_count++;
	if(now_seed_count < count)
		return false;
	if(now_seed_count == count) {
		now_seed_count = 0;
		return true;
	}
	
	return false;

}




void ball_balls(void) {
	static byte start_seed_flag = 0;
	static byte x, y;
	static int8_t speed_x,speed_y;
	
	int one_time_count = 30;
	if(time_counter%60 == 0) {
		randomSeed(millis_get());
		if(activedCount < MAX_BALLS - one_time_count) {
			start_seed_flag = 1;
			// ??��????
			x = random_range(8, 128-8);
			y = random_range(8, 64-8);
			speed_x = random(1)? 1:-1;
			speed_y = random(1)? 1:-1;
		}
	}
	
	if(start_seed_flag) {
		if(seed_ball(one_time_count, x, y, speed_x, speed_y))
			start_seed_flag = false;
	}
	
	activedCount = 0;
	
	for(int i = 0; i < MAX_BALLS; i++) {
		
		if(balls_arry[i].actived) {
			activedCount++;
			// ?m??
			Collide_t collide_state = collide(&balls_arry[i]);
			if(collide_state == UD_C) {
				balls_arry[i].speed_y = -balls_arry[i].speed_y;
			} else if(collide_state == LR_C) {
				balls_arry[i].speed_x = -balls_arry[i].speed_x;
			} else if(collide_state == A_C) {
				balls_arry[i].speed_x = -balls_arry[i].speed_x;
				balls_arry[i].speed_y = -balls_arry[i].speed_y;
			} else if(collide_state == C_C) {
				balls_arry[i].actived = false;
			}
		
			
			// update position
			if(time_counter%(2* (BALL_MAX_SPEED + 1 - abs(balls_arry[i].speed_x))) == 0 || collide_state) {
				balls_arry[i].x += balls_arry[i].speed_x > 0 ? 1:-1;
			}
			
			if(time_counter%(2* (BALL_MAX_SPEED + 1 - abs(balls_arry[i].speed_y))) == 0 ||collide_state) {
				balls_arry[i].y += balls_arry[i].speed_y > 0 ? 1:-1;
			}
			
			// draw ball
			draw_point(balls_arry[i].x, balls_arry[i].y, 1, 0);
			
			
		}
		
	}
	
	time_counter++;

}




/**
 * @file main.c 
 * @author xiaoyang (you@domain.com)
 * @brief 表盘app示例工程
 * @version 0.1
 * @date 2025-08-09
 * 
 * @copyright Copyright (c) 2025
 * 
 */

// 引用系统函数API
#include "watchface_api.h"


#define VECTOR_NUM  1

#define TIME_POS_X	1
#define TIME_POS_Y	20

#if VECTOR_NUM != 0
#define TICKER_GAP	4
#else
#define TICKER_GAP	1
#endif


static display_t draw(void);
void drawDate(void);
display_t ticker(s8 offset_x, s8 offset_y, u8 is_second);
static void drawTickerNum(s8, s8, tickerData_t*);

#if COMPILE_ANIMATIONS
static bool animateIcon(bool, byte*);
#endif





//watch face
void my_main(void)
{
	systemVariableInit();
	display_setDrawFunc(draw);
	open_watchface_general(); // 通用的表盘打开函数
	openLSM6DSM(LSM_TEMP);
}

static display_t draw()
{

	static byte blueToothImagePos = FRAME_HEIGHT;
	static byte chargeImagePos = FRAME_HEIGHT;

	watchFace_switchUpdate();
	
	// Draw date
	drawDate();

	// Draw time animated
	display_t busy ;
	
	busy= ticker(0, 0, 1);

	// Draw battery icon
	byte x = drawBattery(NUM_INVERSE_AND_DEFAULT);
	
	drawTemperature(0);

	// Draw charging icon
	if(animateIcon(getCharingState(), &chargeImagePos))
	{
		draw_bitmap(x, chargeImagePos, chargeIcon, 8, 8, NOINVERT, 0);
		x += 12;
	}	

	
	// bluetooth link state
	if(animateIcon(getBtLinkState(), &blueToothImagePos))
	{
		draw_bitmap(x, blueToothImagePos, blueToothIcon, 8, 8, NOINVERT, 0);
		x += 12;
	}


	// Stopwatch icon
	if(stopwatch_active())
	{
		draw_bitmap(x, FRAME_HEIGHT - 8, stopwatch, 8, 8, NOINVERT, 0);
		x += 12;
	}

	// Draw next alarm
	alarm_s nextAlarm;
	if(alarm_getNext(&nextAlarm))
	{
		time_s alarmTime;
		alarmTime.hour = nextAlarm.hour;
		alarmTime.mins = nextAlarm.min;
		alarmTime.ampm = CHAR_24;
		time_timeMode(&alarmTime, TIMEMODE_24HR);
		draw_bitmap(x, FRAME_HEIGHT - 8, dowImg[alarm_getNextDay()], 8, 8, NOINVERT, 0);
	}
	
	return busy;
}

#if COMPILE_ANIMATIONS
static bool animateIcon(bool active, byte* pos)
{
	byte y = *pos;
	if(active || (!active && y < FRAME_HEIGHT))
	{
		if(active && y > FRAME_HEIGHT - 8)
			y -= 1;
		else if(!active && y < FRAME_HEIGHT)
			y += 1;
		*pos = y;
		return true;
	}
	return false;
}
#endif

display_t ticker(s8 offset_x, s8 offset_y, u8 is_second)
{
	static byte yPos;
	static byte yPos_secs;
	static bool moving = false;
	static bool moving2[5];

#if COMPILE_ANIMATIONS
	static byte hour2;
	static byte mins;
	static byte secs;

	if(appConfig->animations)     
	{
		if(timeDate->time.secs != secs)
		{
			yPos = 0;
			yPos_secs = 0;
			moving = true;

			moving2[0] = div10(timeDate->time.hour) != div10(hour2);
			moving2[1] = mod10(timeDate->time.hour) != mod10(hour2);
			moving2[2] = div10(timeDate->time.mins) != div10(mins);
			moving2[3] = mod10(timeDate->time.mins) != mod10(mins);
			moving2[4] = div10(timeDate->time.secs) != div10(secs);
		
			//memcpy(&timeDateLast, &timeDate, sizeof(timeDate_s));
			hour2 = timeDate->time.hour;
			mins = timeDate->time.mins;
			secs = timeDate->time.secs;
		}

		if(moving)
		{
			if(yPos <= 3)
				yPos++;
			else if(yPos <= 6)
				yPos += 3;
			else if(yPos <= 16)
				yPos += 5;
			else if(yPos <= 22)
				yPos += 3;
			else if(yPos <= MIDFONT_HEIGHT + TICKER_GAP)
				yPos++;

			if(yPos >= MIDFONT_HEIGHT + TICKER_GAP)
				yPos = 255;

			if(yPos_secs <= 2)
				yPos_secs++;
			else if(yPos_secs <= 10)
				yPos_secs += 2;
			else if(yPos_secs <= FONT_SMALL2_HEIGHT + TICKER_GAP)
				yPos_secs++;

			if(yPos_secs >= FONT_SMALL2_HEIGHT + TICKER_GAP)
				yPos_secs = 255;

			if(yPos_secs > FONT_SMALL2_HEIGHT + TICKER_GAP && yPos > MIDFONT_HEIGHT + TICKER_GAP)
			{
				yPos = 0;
				yPos_secs = 0;
				moving = false;
				memset(moving2, false, sizeof(moving2));
			}
		}
	}
	else
#endif
	{
		yPos = 0;
		yPos_secs = 0;
		moving = false;
		memset(moving2, false, sizeof(moving2));
	}

	tickerData_t data;

	// Seconds
	if(is_second) {
		data.x = 104;
		data.y = 28;
//		data.bitmap = (const byte*)&small2Font;
		data.w = FONT_SMALL2_WIDTH;
		data.h = FONT_SMALL2_HEIGHT;
		data.offsetY = yPos_secs;
		data.val = div10(timeDate->time.secs);
		data.maxVal = 5;
		data.moving = moving2[4];
		drawTickerNum(offset_x, offset_y,&data);

		data.x = 116;
		data.val = mod10(timeDate->time.secs);
		data.maxVal = 9;
		data.moving = moving;
		drawTickerNum(offset_x, offset_y,&data);
	} else {
		offset_x += 12;
	}
	
	// Set new font data for hours and minutes
	data.y = TIME_POS_Y;
	data.w = MIDFONT_WIDTH;
	data.h = MIDFONT_HEIGHT;
//	data.bitmap = (const byte*)&midFont;
	data.offsetY = yPos;

	// Minutes
	data.x = 60;
	data.val = div10(timeDate->time.mins);
	data.maxVal = 5;
	data.moving = moving2[2];
	drawTickerNum(offset_x, offset_y,&data);

	data.x = 83;
	data.val = mod10(timeDate->time.mins);
	data.maxVal = 9;
	data.moving = moving2[3];
	drawTickerNum(offset_x, offset_y,&data);

	// Hours
	data.x = 1;
	data.val = div10(timeDate->time.hour);
	data.maxVal = 5;
	data.moving = moving2[0];
	drawTickerNum(offset_x, offset_y,&data);

	data.x = 24;
	data.val = mod10(timeDate->time.hour);
	data.maxVal = 9;
	data.moving = moving2[1];
	drawTickerNum(offset_x, offset_y,&data);
	
	// Draw colon for half a second   ۭѫīքðۅ
	 if(millis_get()%1000>500)   // ݙװˇѫīד  30ms
		draw_bitmap(TIME_POS_X + 46 + 2 + offset_x, TIME_POS_Y + offset_y, colon, FONT_COLON_WIDTH, FONT_COLON_HEIGHT, NOINVERT, 0);
	
	// Draw AM/PM character
	char tmp[2];
	tmp[0] = timeDate->time.ampm;
	tmp[1] = 0x00;
	draw_string(tmp, false, 104+ offset_x, 20+ offset_y);

//	char buff[12];
//	sprintf_P(buff, PSTR("%lu"), time_getTimestamp());
//	draw_string(buff, false, 30, 50);

//	return (moving ? DISPLAY_BUSY : DISPLAY_DONE);
	 return DISPLAY_DONE;
}



static void drawTickerNum(s8 offset_x, s8 offset_y, tickerData_t* data)
{
	byte arraySize = (data->w * data->h) / 8;
	byte yPos = data->offsetY;
	const byte* bitmap = &data->bitmap[data->val * arraySize];
	int16_t x = data->x + offset_x;
	int16_t y = data->y + offset_y;
#if VECTOR_NUM != 0
	uint8_t size_w,size_h;
	if(data->w == FONT_SMALL2_WIDTH) {
		size_w = 2;  // 扩大2倍
		size_h = 2;
		y += 4; // 调整显示
	} else {
		size_w = 4;  // 扩大4倍
		size_h = 4;
		y -= 2; // 调整显示
		if(x==83||x==60) // 分钟
			x -=1 ;
	}
	setVectorDrawClipWin(x, y, x+size_w*5, y+size_h*7);
#endif
	if(!data->moving || yPos == 0 || yPos == 255) {
#if VECTOR_NUM == 0
		draw_bitmap(x, y, bitmap, data->w, data->h, NOINVERT, 0);
//		mydraw_bitmap(x + offset_x, y + offset_y, bitmap, data->w, data->h, NOINVERT, start_x, start_y, end_x, end_y, false);
#else
		drawVectorChar(x, y,'0'+ data->val,1,1,size_w,size_h);
#endif
	} else {
		byte prev = data->val - 1;
		if(prev == 255)
			prev = data->maxVal;

#if VECTOR_NUM == 1
		drawVectorChar(x, y -(yPos - data->h - TICKER_GAP), '0'+ data->val,1,1,size_w,size_h);
		drawVectorChar(x, y - yPos,'0'+ prev,1,1,size_w,size_h);
#elif VECTOR_NUM == 2
		int16_t bias = 3;
		int16_t cut_value = yPos -bias;
		
		// last
		setVectorDrawClipWin(x, y, x+size_w*5, y+ cut_value);
		drawVectorChar(x , y ,'0'+ data->val,1,1,size_w,size_h);
		
		myOLED_DrawLine(x -1, y  + cut_value, x + size_w*5, y + cut_value, 1, 0);
		
//		s16 aaa;
//		if(data->w == FONT_SMALL2_WIDTH)
//			aaa=(data->w)/2;
//		else
//			aaa=(data->w);
//		s16 delt_x = (data->w+3)*(data->w) * yPos/(data->h + TICKER_GAP) % (data->w+3);
		s16 delt_x = yPos%2==0?0: (data->w+3);
		
//		s16 delt_x = rand() % (data->w+3);
		myOLED_DrawLine(64, 63, x-1 + delt_x, y + cut_value, 1, 0);
		// new
		setVectorDrawClipWin(x, y + cut_value, x+size_w*5, y+data->h + TICKER_GAP);
		drawVectorChar(x , y , '0'+ prev,1,1,size_w,size_h);	
#endif
		
	}	
}

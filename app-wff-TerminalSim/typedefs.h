/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef TYPEDEFS_H_
#define TYPEDEFS_H_

#include <stdbool.h>
#include <stdint.h>

 #if defined ( __CC_ARM   )
    #pragma anon_unions
 #endif

typedef uint8_t byte;
typedef uint16_t uint;
typedef uint32_t ulong;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t s8;
typedef int16_t s16;
typedef int32_t s32;

extern byte oledBuffer[];


typedef unsigned int millis8_t;
typedef unsigned int  millis_t;

// bool from stdbool does extra stuff to make the value
// always either 0 or 1, which isn't needed most of the time.
// So here's BOOL which doesn't do that.
typedef	uint8_t BOOL;

typedef uint32_t timestamp_t;

typedef enum
{
	DISPLAY_DONE,
	DISPLAY_BUSY,
//	DISPLAY_TOOFAST
} display_t;

typedef enum
{
	MENU_TYPE_STR,
	MENU_TYPE_ICON
} menu_type_t;

typedef enum
{
	MONTH_JAN = 0,
	MONTH_FEB = 1,
	MONTH_MAR = 2,
	MONTH_APR = 3,
	MONTH_MAY = 4,
	MONTH_JUN = 5,
	MONTH_JUL = 6,
	MONTH_AUG = 7,
	MONTH_SEP = 8,
	MONTH_OCT = 9,
	MONTH_NOV = 10,
	MONTH_DEC = 11
} month_t;

typedef enum
{
	DAY_MON = 0,
	DAY_TUE = 1,
	DAY_WED = 2,
	DAY_THU = 3,
	DAY_FRI = 4,
	DAY_SAT = 5,
	DAY_SUN = 6,
} day_t;

typedef struct {
	byte secs;
	byte mins;
	byte hour;
	char ampm;
}time_s;

typedef struct {
	day_t day;
	byte date;
	month_t month;
	byte year;
}date_s;

typedef struct {
	time_s time;
	date_s date;
}timeDate_s;

typedef enum
{
	TIMEMODE_24HR = 0,
	TIMEMODE_12HR = 1
} timemode_t;

typedef struct{
	byte hour;
	byte min;
	union {
		byte days; 
		struct{ // get rid of these bitfields?
			bool mon:1;
			bool tues:1;
			bool wed:1;
			bool thurs:1;
			bool fri:1;
			bool sat:1;
			bool sun:1;
			bool enabled:1;
		};
	};
}alarm_s;

typedef enum {
	DOUBLE_TAP_FUNC = 1,
	TILT_FUNC = 1<<1,
	PEDOMETER_FUNC = 1<<2,
} SensorExtendFunc_t;

typedef struct {
	char date[6]; // 10/10
	uint16_t pedometerData;
}PedometerData_s;

enum {
	ENGLISH,
	CHINESE,
};

// Could use bitfields for the bools to save a few bytes of RAM and EEPROM, but uses an extra ~82 bytes of flash
typedef struct{
//	uint16_t check_num;
	byte sleepTimeout;
	byte brightness;
	bool invert;
#if COMPILE_ANIMATIONS
	bool animations;
#endif
	//byte clockface;
	bool display180;
	bool CTRL_LEDs;
	bool showFPS;
	byte fps;
	byte lang;
	SensorExtendFunc_t sensor_extendfunc;
	timemode_t timeMode;
	union {
		ulong volumes;
		struct{ // get rid of these bitfields?
			byte volUI;
			byte volGame;
			byte volAlarm;
			byte volHour;
		};
	};
	// alsms
	alarm_s alarms[10];
	// pedometer
	PedometerData_s pedo[7];
}appconfig_s;

typedef display_t (*draw_f)(void);
typedef void (*display_f)(void);

// Function for buttons to call
// Return true to only call function once per press
// Return false to continually call function while the button is pressed

//定义一个返回值为bool的函数指针
typedef bool (*button_f)(void);

typedef void (*menu_f)(void);
typedef void (*itemLoader_f)(byte);

typedef struct{
	menu_f btn1; // make array
	menu_f btn2;
	menu_f btn3;
	draw_f draw;
	itemLoader_f loader;
}menuFuncs_t;

typedef enum
{
	NOT_SET = 0,
	DIRECT,
	OPEN_UP ,
	OPEN_DOWN,
	CLOSE_UP,
	CLOSE_DOWN,
	
	OPEN_R,
	OPEN_L,
	CLOSE_R,
	CLOSE_L,
	BLANK,
} animMode_t;

typedef enum
{
	MENU_POP,
	MENU_PUSH
} menuLeaveState_t;

typedef struct{
	byte selected;  // 当前菜单选项
	byte lastSelected;  // 当前菜单的上次选项
	byte scroll;
	byte optionCount;
	bool isOpen;
	const char* title;
	menu_type_t menuType;
	menuFuncs_t func;
	menu_f prevMenu;
}menu_s;

typedef struct{
	byte lastSelected;
	menu_f last;
	menuLeaveState_t menuLeaveState;
}prev_menu_s;



typedef struct{
	bool active;             //开关
	signed short offsetY;
	signed short offsetX;
	void (*animOnComplete)(void);    //指向的函数
	animMode_t mode;  // 动画模式
}anim_s;




typedef struct{
	int16_t x;
	int16_t y;
	const byte* bitmap;
	byte width;
	byte height;
//	byte foreColour;
	bool invert;
	byte offsetY;
}image_s;

#endif /* TYPEDEFS_H_ */

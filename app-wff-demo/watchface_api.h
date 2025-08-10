#ifndef _WATCHFACE_H_
#define _WATCHFACE_H_



#include "config.h"
#include "typedefs.h"
#include "util.h"
#include "english.h"
#include "resources.h"


/* gloable variable typdefs */
typedef enum {
	DEFAULT = 0,
	NUM_ONLY,
	NUM_INVERSE_ONLY,
	NUM_AND_DEFAULT,
	NUM_INVERSE_AND_DEFAULT,
} drawBatMode_t;

typedef enum {
	LSM_NONE = 0,
	LSM_TEMP = 1,
	LSM_ACC_GYRO = 1<<1,

} Lsm6dsmMode_t;

typedef struct
{
	byte x;
	byte y;
	const byte* bitmap;
	byte w;
	byte h;
	byte offsetY;
	byte val;
	byte maxVal;
	bool moving;
}tickerData_t;


/* system varibales*/
extern timeDate_s *timeDate;
extern appconfig_s *appConfig;
extern byte *colon;
extern byte (*dowImg)[7][8];
extern byte *stopwatch;
extern byte *blueToothIcon;
extern byte *chargeIcon;


/* system functions */
void time_timeMode(time_s*, timemode_t);
bool alarm_getNext(alarm_s* alarm);
void draw_string(char*, bool, s16, s16);
byte alarm_getNextDay(void);
void draw_bitmap(s16 x, s16 yy, const byte* bitmap, byte w, byte h, bool invert, byte offsetY);
byte div10(byte val);
byte mod10(byte val);
void drawDate(void);
static void drawTickerNum(s8, s8, tickerData_t*);
void console_log(unsigned short time_delay ,char* fmt,...);
void drawVectorChar(int16_t x, int16_t y, unsigned char c,
						uint16_t color, uint16_t bg, uint8_t size_x,
						uint8_t size_y);
byte getBtLinkState(void);
byte getCharingState(void);
bool stopwatch_active(void);

#endif


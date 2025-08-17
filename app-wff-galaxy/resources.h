/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

#ifndef RESOURCES_H_
#define RESOURCES_H_
#include "typedefs.h"
#include "english.h"

extern const byte menu_default[];
extern const byte menu_alarm[];
extern const byte count_down[];
extern const byte pedometer[];
extern const byte compass_icon[];
extern const byte menu_stopwatch[];
extern const byte menu_torch[];
extern const byte menu_settings[];
extern const byte battery_icon[];
extern const byte txtViewer_icon[];
extern const byte filemg_icon[];
extern const byte menu_diagnostic[];
extern const byte menu_exit[];
extern const byte selectbar_bottom[];
extern const byte selectbar_top[];
extern const char fileDirIcon[];
extern const char fileUnkonwIcon[]; 
extern const char txtFileIcon[];
extern const char appFileIcon[];
extern const char dowChars[];
extern const byte tempAlt_icon[];

extern const char days[7][BUFFSIZE_STR_DAYS];
extern const char months[12][BUFFSIZE_STR_MONTHS];

extern const char *days_zh[7];
extern const char *months_zh[12];

extern const byte livesImg[];
// extern const byte stopwatch[];

// extern const byte dowImg[7][8];

extern const byte menu_tunemaker[];

extern const byte menu_timedate[];
extern const byte menu_sleep[];
extern const byte menu_sound[];
extern const byte menu_games[];
//extern const byte menu_calc[];
extern const byte menu_brightness[][128];
extern const byte menu_LEDs[][128];
extern const byte menu_invert[];
extern const byte menu_anim[][128];
extern const byte menu_volume[][128];
extern const byte menu_rotate[];
extern const byte menu_display[];
extern const byte menu_sleeptimeout[];

extern const byte usbIcon[];
// extern const byte chargeIcon[];
// extern const byte blueToothIcon[];

extern const byte battIconEmpty[];
extern const byte battIconLow[];
extern const byte battIconHigh[];
extern const byte battIconFull[];

//锟揭碉拷锟斤拷锟斤拷锟斤拷图锟斤拷
extern const byte menu_setfps[];
// Alarm icon
//extern const byte smallFontAlarm[];

#define SMALLFONT_WIDTH 5
#define SMALLFONT_HEIGHT 8
//extern const byte smallFont[][5];

#define MIDFONT_WIDTH 19
#define MIDFONT_HEIGHT 24  // 24

extern const byte midFont[][57];

#define SEGFONT_WIDTH 19
#define SEGFONT_HEIGHT 24
extern const byte segFont[][57];

#define FONT_SMALL2_WIDTH 11
#define FONT_SMALL2_HEIGHT 16  // 16

//extern const byte small2Font[][22];

#define FONT_COLON_WIDTH 6
#define FONT_COLON_HEIGHT 24
// extern const byte colon[];

#endif /* RESOURCES_H_ */

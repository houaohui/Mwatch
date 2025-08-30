#include "watchface_api.h"

/* system variables */
timeDate_s *timeDate;  	extern timeDate_s *timeDate_sysVar();
appconfig_s *appConfig;	extern  appconfig_s *appConfig_sysVar();
byte *colon;  			extern byte *colon_sysVar();
byte (*dowImg)[7][8] ;  extern byte (*dowImg_sysVar(void))[7][8];
byte *stopwatch;  		extern byte *stopwatch_sysVar();
byte *blueToothIcon;  	extern byte *blueToothIcon_sysVar();
byte *chargeIcon;  		extern byte *chargeIcon_sysVar();
byte (*smallFont)[97][5];  extern byte (*smallFont_sysVar(void))[97][5];
byte *oledBuffer;  		extern byte *oledBuffer_sysVar(void);

/**
 * @brief system gloable variable rename  get gloable variable address
 * Like using variables in system project
 */
void systemVariableInit(void) {
    blueToothIcon = blueToothIcon_sysVar();
    chargeIcon = chargeIcon_sysVar();
    stopwatch = stopwatch_sysVar();
    dowImg = dowImg_sysVar();
    colon = colon_sysVar();
    timeDate = timeDate_sysVar();
    appConfig = appConfig_sysVar();
    smallFont = smallFont_sysVar();
	oledBuffer = oledBuffer_sysVar();
}











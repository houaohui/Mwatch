#include "watchface_api.h"

/* system variables */
timeDate_s *timeDate;
appconfig_s *appConfig;
byte *colon;
byte (*dowImg)[7][8] ;
byte *stopwatch;
byte *blueToothIcon;
byte *chargeIcon;
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
}











/*
 * Project: N|Watch
 * Author: Zak Kemble, contact@zakkemble.co.uk
 * Copyright: (C) 2013 by Zak Kemble
 * License: GNU GPL v3 (see License.txt)
 * Web: http://blog.zakkemble.co.uk/diy-digital-wristwatch/
 */

// Fuses for ATmega328/328P
// Internal RC 8MHz 6 CK/14CK + 4.1ms startup
// Serial program (SPI) enabled
// Brown-out at 1.8V
// High:		0xDF
// Low:			0xD2
// Extended:	0xFE

#ifndef CONFIG_H_
#define CONFIG_H_

//#define CPU_DIV clock_div_1

// Hardware version
// 1 = PCB 1.0 - 1.1
// 2 = PCB 1.2
// 3 = PCB 1.3 - 1.4

//#define HW_VERSION	2

#define TXTREADER_INTERNAL   0

// arduboy games configs
#define ARDUBOY   1
#define ARDUBOY_EVASION      0
#define ARDUBOY_ARDUBULLETS  0
#define ARDUBOY_HOLLOW       0
#define ARDUBOY_REVERSI      0
#define ARDUBOY_LASERS       0
#define ARDUBOY_SAMEGAME     0

#define CODERAIN_WATCHFACE 1
#define TERMINALSIM_WATCHFACE 1
#define SIMCLOCK_WATCHFACE  1
#define CUDE3D_WATCHFACE 1
#define SIMPLECLOCK_WATCHFACE 1
#define WAVES_WATCHFACE 1
#define BALLS_WATCHFACE 1
#define SAND_WATCHFACE 1
#define SNOW_WATCHFACE 1
#define GALAXY_WATCHFACE 1
#define NENO_WATCHFACE 1

#define USER_NAME "xxx"

// Firmware version
#define FW_VERSION	"   2025/5/1" // maybe use some __DATE__ __TIME__ stuff?

// Language
// 0 = English
// 1 = German (not done)
// 2 = Russian
#define LANGUAGE 0


// ����ѡ��
#define COMPILE_GAME1      1 // ��ϷBreakout    
#define COMPILE_GAME2      1 // ��ϷCar dodge
#define COMPILE_GAME3      1 // ��ϷFlappy thing
#define COMPILE_ANIMATIONS 1 //����
#define COMPILE_STOPWATCH  1 //���
#define COMPILE_PEDOMETER  1 //�Ʋ���
#define COMPILE_TORCH      1 //�ֵ�Ͳ
#define COMPILE_TUNEMAKER  1 //3D����

#define COMPILE_BATTERY_HISTORY  1  // ���ҳ��


//���������������Դ��ڴ�ӡ������Ϣ
#define COMPILE_UART       0
#define DEBUG_MSGS		   0
#define UART_BAUD          115200



#define RTC_SRC  1 // �Ƿ�ʹ���ⲿʱ��Դ
#define RTCFUNC_PREFIX	stm32rtc_  // ʹ��stm32��rtc


#endif /* CONFIG_H_ */

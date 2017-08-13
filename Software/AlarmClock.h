// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _AlarmClock_H_
#define _AlarmClock_H_

#include "config.h"
#include "Alarmmanagement.h" /* for s_alarm struct */
#include "sensormanagement.h"
#include "ledManagement.h"
#include "time_esp8266.h"
#include "TimeManagement.h"
#include "TM1637Display.h"
#include "timer.h"
#include "eeprom.h"
#include "myWifiForTime.h"
#include "utils.h"


typedef enum  {
	e_UI_default, //default : time display
	e_UI_alarm_display,	//display current alarm
	e_UI_alarm_selection, //cycle/program alarm
	e_UI_MAX
} e_state_UI;

//Do not add code below this line
#endif /* _AlarmClock_H_ */

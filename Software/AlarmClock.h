// Only modify this file to include
// - function definitions (prototypes)
// - include files
// - extern variable definitions
// In the appropriate section

#ifndef _AlarmClock_H_
#define _AlarmClock_H_
#include "Arduino.h"
#include "Alarmmanagement.h" /* for s_alarm struct */


#define PIN_LED 2 //and D4??
#define PIN_TX  1
#define PIN_RX  3

#define PIN_D0	16
#define PIN_D1	5
#define PIN_D2	4
#define PIN_D3	0
#define PIN_D4	PIN_LED
#define PIN_D5	14
#define PIN_D6	12
#define PIN_D7	13
#define PIN_D8	15


#define PIN_WIRE_SDA PIN_D2 //4
#define PIN_WIRE_SCL PIN_D1 //5

#define PIN_PARAM_CONFIG PIN_D5	//launch the adhoc WIFI

#define ERROR(file,line)	({char s[100]; sprintf(s,"ERROR %s:%d",file, line); Serial.println(s);})

#define WITHOUT_UI

#define VERBOSE_ACTIVATED
#ifdef VERBOSE_ACTIVATED
	#define VERBOSE(a)	(a)
#else
	#define VERBOSE(a)	()
#endif

#define AFFSEG_PIN_CLK	PIN_D5
#define AFFSEG_PIN_DIO	PIN_D6

#define NBR_OF_TILTS_TO_WAKE_UP	1
#define NBR_OF_TILTS_TO_SWITCH_TO_ALARM_MANAGEMENT	1
#define NBR_OF_TILTS_TO_WAKE_UP_TO_ALARM_MANAGEMENT	2

#define MAX_TIME_SHOWING_CURRENT_ALARM		2000 //in ms
#define MAX_TIME_SHOWING_ALARM_SELECTION	5000 //in ms
#define MIN_TIME_BETWEEN_INPUTS				300 //in ms

#define MAX_TIME_TO_RING	1 // in s : duration wich trigger the alarm (if defined to 10:00.0s, ring if polled until 10:00.1s)

#define CONST_SSID_SIZE 33 /* up to 32 char */
#define CONST_PWD_SIZE 65 /* up to 64 char */

typedef struct {
	long non_random_string; /* value if something is stored */
	int alarm_hours;
	int alarm_minutes;
	bool valid_wifi_credentials;
	char wifi_ssid[CONST_SSID_SIZE];
	char wifi_psswd[CONST_PWD_SIZE];
	s_alarm predef_alarms[NBR_OF_PREDEFINED_ALARMS];
} t_struct_in_eeprom;

#define SIZE_OF_STRUCT_IN_EEPROM sizeof(t_struct_in_eeprom)
#define NON_RANDOM_STRING	0xA5A6


//Do not add code below this line
#endif /* _AlarmClock_H_ */

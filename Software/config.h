#ifndef CONFIG_H_
#define CONFIG_H_

#include "Arduino.h"

/*********/
/* DEBUG */
/*********/

#define ERROR(file,line)	({char s[100]; sprintf(s,"ERROR %s:%d",file, line); Serial.println(s);})

//#define WITHOUT_UI

#define VERBOSE_ACTIVATED
#ifdef VERBOSE_ACTIVATED
	#define VERBOSE(a)	(a)
#else
	#define VERBOSE(a)	()
#endif

/********/
/* PINS */
/********/

#define PIN_TX  1
#define PIN_RX  3

#define PIN_D0	16
#define PIN_D1	5
#define PIN_D2	4
#define PIN_D3	0
#define PIN_D4	2
#define PIN_D5	14
#define PIN_D6	12
#define PIN_D7	13
#define PIN_D8	15

#define PIN_LED PIN_D4


#define PIN_WIRE_SDA PIN_D2 //4
#define PIN_WIRE_SCL PIN_D1 //5

/***********/
/* SENSORS */
/***********/
#define PIN_LUMINOSITY	A0
#define MIN_INPUT_VALUE_LUM 130
#define MAX_INPUT_VALUE_LUM 1000

/********/
/* LEDS */
/********/

/* TODO : proper affectation for PIN_BATTERY */
#define PIN_BATTERY_LED PIN_D0
#define PIN_WIFI_LED PIN_D3
#define PIN_ALARM_LED PIN_D4

/*************/
/* LEDSCREEN */
/*************/

#define AFFSEG_PIN_CLK	PIN_D5
#define AFFSEG_PIN_DIO	PIN_D6

#define ALARM_BLINK_PERIOD_MS	700

/*********/
/* ALARM */
/*********/

#define NBR_OF_TILTS_TO_WAKE_UP	1
#define NBR_OF_TILTS_TO_SWITCH_TO_ALARM_MANAGEMENT	1
#define NBR_OF_TILTS_TO_WAKE_UP_TO_ALARM_MANAGEMENT	2

#define MAX_TIME_SHOWING_CURRENT_ALARM		2000 //in ms
#define MAX_TIME_SHOWING_ALARM_SELECTION	5000 //in ms
#define MIN_TIME_BETWEEN_INPUTS				300 //in ms

#define MAX_TIME_TO_RING	2 // in s : duration wich trigger the alarm (if defined to 10:00.0s and 1s delay to ring, ring if polled until 10:00.1s)

/**************/
/* WIFI PARAM */
/**************/

#define MAX_TIME_WAITING_FOR_WIFI_MS	15000 //time to test if settings are corrects

#define NIST_TIME_SERVER	129,6,15,28  // time.nist.gov NTP server

#define PIN_PARAM_CONFIG PIN_D7	//launch the adhoc WIFI if set to ground at startup
#define AD_HOC_WIFI_ADRESS 42,42,42,42

#endif

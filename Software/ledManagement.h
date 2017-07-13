/*
 * ledManagement.h
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */

#ifndef LEDMANAGEMENT_H_
#define LEDMANAGEMENT_H_

#include "Arduino.h"
#include "AlarmClock.h"

/* TODO : proper affectation for PIN_BATTERY */
#define PIN_BATTERY_LED PIN_D0
#define PIN_WIFI_LED PIN_D3
#define PIN_ALARM_LED PIN_D4

class LEDs_management {
private:
	int last_luminosity;
	bool battery_state;
	bool wifi_state;
	bool alarm_state;
	void refresh_leds();
	int get_pwm();




public:
	LEDs_management();
	void update(int luminosity, bool battery_on, bool wifi_on, bool alarm_on);
};



#endif /* LEDMANAGEMENT_H_ */

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


/* TIMING */
#define COUNT_BLINKING	100
#define COUNT_FADING_FAST	1
#define COUNT_FADING_SLOW	3

typedef enum {
	e_state_led_fixed,
	e_state_led_blinking,
	e_state_led_fading_fast,
	e_state_led_fading_slow
} e_state_led;

class LEDs_management {
private:
	LED *led_alarm;
	LED *led_battery;
	LED *led_wifi;
	void refresh_leds(int luminosity);

public:
	LEDs_management();
	void update(int luminosity, bool battery_on, bool wifi_on, bool alarm_on);
	void setModeAlarmLed(e_state_led expected_state);
	void setModeWifiLed(e_state_led expected_state);
	void setModeBatteryLed(e_state_led expected_state);
};

class LED {
private:
	int pin;
	bool state;
	e_state_led mode;
	float get_pwm(int luminosity);

public:
	LED(int pin_in);
	void setMode(e_state_led mode_in);
	void setState(boolean state_in);
	void refresh_led(int lum);
};

#endif /* LEDMANAGEMENT_H_ */

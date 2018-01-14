/*
 * ledManagement.h
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */

#ifndef LEDMANAGEMENT_H_
#define LEDMANAGEMENT_H_

#include "Arduino.h"
#include "config.h"


/* TIMING */
#define BLINKING_PERIOD_MS		900
#define BLINKING_FAST_PERIOD_MS	500
#define FADING_PERIOD_MS		1500
#define FADING_FAST_PERIOD_MS	900

/* PWM */
#define MIN_PWM_VALUE 6.0

typedef enum {
	e_state_led_fixed,
	e_state_led_blinking_fast,
	e_state_led_blinking,
	e_state_led_fading_fast,
	e_state_led_fading
} e_state_led;


class LED {
private:
	int pin;
	bool state;
	int last_pwm;
	bool last_state;
	e_state_led mode;
	u8 get_pwm(int luminosity, float modifier);

public:
	LED(int pin_in);
	void setMode(e_state_led mode_in);
	void setState(boolean state_in);
	void refresh_led(int lum);
};

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


#endif /* LEDMANAGEMENT_H_ */

/*
 * ledManagement.cpp
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */


#include "ledManagement.h"

LEDs_management::LEDs_management() {
	last_luminosity = 	-1;
	battery_state	=	false;
	wifi_state		=	false;
	alarm_state		=	false;

	pinMode(PIN_BATTERY_LED, OUTPUT);
	pinMode(PIN_WIFI_LED, OUTPUT);
	pinMode(PIN_ALARM_LED, OUTPUT);

	this->refresh_leds();
}

void LEDs_management::update(int luminosity, bool battery_on, bool wifi_on, bool alarm_on) {
	bool refresh_needed = false;

	if(luminosity != last_luminosity){
		refresh_needed=true;
		last_luminosity = luminosity;
	}

	if(battery_on != battery_state){
		refresh_needed=true;
		battery_state = battery_on;
	}

	if(wifi_on != wifi_state){
		refresh_needed=true;
		wifi_state = wifi_on;
	}

	if(alarm_on != alarm_state){
		refresh_needed=true;
		alarm_state = alarm_on;
	}

	if(refresh_needed){
		this->refresh_leds();
	}

}

void LEDs_management::refresh_leds() {
	analogWrite(PIN_BATTERY_LED, this->get_pwm()*(int)battery_state);
	analogWrite(PIN_WIFI_LED, this->get_pwm()*(int)wifi_state);
	analogWrite(PIN_ALARM_LED, this->get_pwm()*(int)alarm_state);
}

int LEDs_management::get_pwm() {
	// TODO return proper value correlated to the luminosity
	return 255;
}

/*
 * ledManagement.cpp
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */


#include "ledManagement.h"

LEDs_management::LEDs_management() {
	static LED myAlarm = LED(PIN_ALARM_LED);
	static LED myWifi = LED(PIN_WIFI_LED);
	static LED myBattery = LED(PIN_BATTERY_LED);

	led_alarm = &myAlarm;
	led_wifi = &myWifi;
	led_battery = &myBattery;

	//this->refresh_leds(0);
}

void LEDs_management::update(int luminosity, bool battery_on, bool wifi_on, bool alarm_on) {
	led_alarm->setState(alarm_on);
	led_wifi->setState(wifi_on);
	led_battery->setState(battery_on);

	this->refresh_leds(luminosity);
}

void LEDs_management::refresh_leds(int luminosity) {
	led_alarm->refresh_led(luminosity);
	led_battery->refresh_led(luminosity);
	led_wifi->refresh_led(luminosity);
}

void LEDs_management::setModeAlarmLed(e_state_led expected_state){
	led_alarm->setMode(expected_state);
}

void LEDs_management::setModeBatteryLed(e_state_led expected_state){
	led_battery->setMode(expected_state);
}

void LEDs_management::setModeWifiLed(e_state_led expected_state){
	led_wifi->setMode(expected_state);
}



LED::LED(int pin_in){
	state = false;
	pin=pin_in;
	mode=e_state_led_fixed;
	last_state=false;
	last_pwm=-1;

	pinMode(pin, OUTPUT);
	analogWrite(pin, 0);

}
void LED::setMode(e_state_led mode_in){
	mode = mode_in;
}
void LED::setState(boolean state_in){
	state = state_in;
	if((state == false) && (mode != e_state_led_fixed)){
		mode = e_state_led_fixed;
	}
}
void LED::refresh_led(int lum){

	float modifier = 1;
	int count;

	switch (mode) {
	case e_state_led_fixed:
		// do nothing
		break;
	case e_state_led_blinking_fast:
		if ( (millis()%BLINKING_FAST_PERIOD_MS) < (BLINKING_FAST_PERIOD_MS/2) ){
			modifier = 0;
		}
		break;

	case e_state_led_blinking:
		if ( (millis()%BLINKING_PERIOD_MS) < (BLINKING_PERIOD_MS/2) ){
			modifier = 0;
		}
		break;

	case e_state_led_fading_fast:
		count = millis()%FADING_FAST_PERIOD_MS;
		if ( count < (FADING_FAST_PERIOD_MS/2) ){
			modifier = (float)count / (float)(FADING_FAST_PERIOD_MS/2.0);
		} else {
			modifier = (float)(FADING_FAST_PERIOD_MS - count) / (float)(FADING_FAST_PERIOD_MS/2.0);
		}
		break;

	case e_state_led_fading:
		count = millis()%FADING_PERIOD_MS;
		if ( count < (FADING_PERIOD_MS/2) ){
			modifier = (float)count / (float)(FADING_PERIOD_MS/2.0);
		} else {
			modifier = (float)(FADING_PERIOD_MS - count) / (float)(FADING_PERIOD_MS/2.0);
		}
		break;
	}

	int pwm = get_pwm(lum,modifier);
	if((pwm != last_pwm) || (state != last_state)){
		last_pwm = pwm;
		last_state = state;
		analogWrite(pin, pwm *(int)state);
	}
}

/* lmuminosity between 0 & 100 , modifier between 0 & 1*/
int LED::get_pwm(int luminosity, float modifier) {
	// TODO return proper value correlated to the luminosity
	// between 0 & 255
	return (int)(255.0*modifier);
}

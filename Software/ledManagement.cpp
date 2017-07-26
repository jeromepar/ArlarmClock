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

	this->refresh_leds(0);
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

	pinMode(pin, OUTPUT);
	analogWrite(pin, 0);

}
void LED::setMode(e_state_led mode_in){
	mode = mode_in;
}
void LED::setState(boolean state_in){
	state = state_in;
	if(state == false){
		mode = e_state_led_fixed;
	}
}
void LED::refresh_led(int lum){
	static boolean last_state=0;
	static int last_lum=-1;
	static unsigned int count = 0;
	static float last_modifier = 0;

	float modifier = 1;
	bool force_update = false;

	switch (mode) {
	case e_state_led_fixed:
		// do nothing
		break;
	case e_state_led_blinking:
		if ( (count%(2*COUNT_BLINKING)) < COUNT_BLINKING){
			modifier = 0;
		}
		break;
	case e_state_led_fading_fast:
		if ( (count%COUNT_FADING_FAST)==0){
			if(last_modifier < 0.99) {
				modifier = last_modifier+0.01;
			} else {
				modifier = last_modifier-0.01;
			}
		}
		break;
	case e_state_led_fading_slow:
		break;
	}

	if((modifier != last_modifier) || (lum != last_lum) || (state != last_state)){
		last_lum = lum;
		last_state = state;
		analogWrite(pin, (int)(get_pwm(lum)*(float)state));
	}
	count++;
}
float LED::get_pwm(int luminosity) {
	// TODO return proper value correlated to the luminosity
	// between 0 & 255
	return 255.0;
}

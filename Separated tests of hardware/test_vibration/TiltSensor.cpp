/*
  TiltSensor.h - TiltSensor library
  J. Parisot. 
*/


#if ARDUINO >= 100
#include <Arduino.h> 
#else
#include <WProgram.h> 
#endif

#include "TiltSensor.h"

TiltSensor::TiltSensor() {

	has_been_triggered=false;
	i_am_still_counting_tilts=false;

	time_when_first_tilted = 0;
	time_when_last_tilted = 0;

	cnt_impulses = 0;
}

boolean TiltSensor::watch(int in_value, int *count_OUT) {
	boolean is_result = false;
	*count_OUT = 0;

	if(in_value == TRIGGERING_VALUE)
	{
		time_when_last_tilted = millis();
		if(has_been_triggered == false) {
			has_been_triggered = true;
			time_when_first_tilted = millis();
		}

		if(i_am_still_counting_tilts == false){
			i_am_still_counting_tilts = true;
		}
	} else if (has_been_triggered && (millis()-time_when_last_tilted>SET_DOWN_TIME)) {
		has_been_triggered = false;
		cnt_impulses++;
		time_when_last_tilted=millis();
#ifdef VERBOSE
		Serial.print("End of Impulsion : ");
		Serial.print(millis()-time_when_first_tilted);
		Serial.println("ms");
#endif

	} else if(i_am_still_counting_tilts &&  (millis()-time_when_last_tilted>MAX_TIME_BETWEEN_COUNTS)) {

		i_am_still_counting_tilts = false;

		*count_OUT = cnt_impulses;
		cnt_impulses=0;
		is_result = true;
	}
	return(is_result);
}

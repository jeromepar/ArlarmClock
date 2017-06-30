/*
 * sensormanagement.cpp
 *
 *  Created on: 25 juin 2017
 *      Author: jj
 */

#include "sensormanagement.h"

Sensor_management::Sensor_management() {
	tilt_sensor_nbr_of_shakes = 0;
}

void Sensor_management::update_data() {

	if (every500ms.watch())
	{
		//Refresh luminosity
	}

	if (every100ms.watch())
	{
		//Refresh Battery status & accelerometer
	}

	if (every20ms.watch())
	{
		//Refresh TiltSensor
		int nbr_of_shakes;
		bool new_data = tilt_sensor.watch(digitalRead(TILT_SENSOR_PIN), &nbr_of_shakes);

		/* if new count and previous one already used */
		if(new_data && tilt_sensor_nbr_of_shakes == 0 )
		{
			tilt_sensor_nbr_of_shakes = nbr_of_shakes;
		}
	}
}

int Sensor_management::get_tilt_sensor_shakes(){
	int return_value = tilt_sensor_nbr_of_shakes;
	tilt_sensor_nbr_of_shakes = 0;
	return (return_value);
}

int Sensor_management::get_luminosity(){
	return (100);
}

bool Sensor_management::get_battery_use(){
	return (false);
}

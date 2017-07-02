/*
 * sensormanagement.h
 *
 *  Created on: 25 juin 2017
 *      Author: jj
 */

#ifndef SENSORMANAGEMENT_H_
#define SENSORMANAGEMENT_H_

#include "Arduino.h"
#include "AlarmClock.h"
#include "TiltSensor.h"
#include "CyclicTimer.h"

#define TILT_SENSOR_PIN  PIN_D1

class Sensor_management {
private:
	TiltSensor tilt_sensor;
	CyclicTimer every500ms=CyclicTimer(500);
	CyclicTimer every100ms=CyclicTimer(100);
	CyclicTimer every20ms=CyclicTimer(20);
	int tilt_sensor_nbr_of_shakes;

public:
	Sensor_management();
	void update_data();
	int get_tilt_sensor_shakes();
	int get_luminosity();
	bool get_battery_use();
};

#endif /* SENSORMANAGEMENT_H_ */

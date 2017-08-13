/*
 * sensormanagement.h
 *
 *  Created on: 25 juin 2017
 *      Author: jj
 */

#ifndef SENSORMANAGEMENT_H_
#define SENSORMANAGEMENT_H_

#include "Arduino.h"
#include "config.h"
#include "ADXL345.h"
#include "CyclicTimer.h"

#define VALUE_ON_G_TO_SWITCH_Z_ORIENTATION (ADXL345_GRAVITY_EARTH/2.0f)
#define VALUE_ON_G_TO_SWITCH_Y_ORIENTATION (ADXL345_GRAVITY_EARTH/3.0f)

typedef enum {
	e_orientationZ_head_up,
	e_orientationZ_head_down,
	e_orientationZ_MAX
} e_orientationZ;

typedef enum {
	e_orientationY_on_left,
	e_orientationY_on_right,
	e_orientationY_none,
	e_orientationY_MAX
} e_orientationY;

class Sensor_management {
private:
	ADXL345 accelerometer;
	e_orientationZ orientationZ;
	e_orientationY orientationY;
	bool init_done;

public:
	Sensor_management();
	void update_data();
	int get_tilt_sensor_shakes();
	int get_luminosity();
	bool get_battery_use();
	e_orientationZ get_Zorientation();
	e_orientationY get_Yorientation();
	void init();
};

#endif /* SENSORMANAGEMENT_H_ */

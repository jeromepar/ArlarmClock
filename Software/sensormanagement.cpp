/*
 * sensormanagement.cpp
 *
 *  Created on: 25 juin 2017
 *      Author: jj
 */

#include "sensormanagement.h"

Sensor_management::Sensor_management() {

	//init orientation
	orientationY=e_orientationY_none;
	orientationZ=e_orientationZ_head_up;
}

void Sensor_management::init() {
	if (!accelerometer.begin())
	{
		VERBOSE(Serial.println("Could not find a valid ADXL345 sensor, check wiring!"));
		delay(500);
	}

	// Set tap detection on Z-Axis
	accelerometer.setTapDetectionX(0);       // Don't check tap on X-Axis
	accelerometer.setTapDetectionY(0);       // Don't check tap on Y-Axis
	accelerometer.setTapDetectionZ(1);       // Check tap on Z-Axis

	accelerometer.setTapThreshold(2.5);      // Recommended 2.5 g
	accelerometer.setTapDuration(0.02);      // Recommended 0.02 s
	accelerometer.setDoubleTapLatency(0.10); // Recommended 0.10 s
	accelerometer.setDoubleTapWindow(0.40);  // Recommended 0.30 s

	Vector min_values={-10.36,-10.71,-8.71};
	Vector max_values={10.20,10.08,11.06};
	accelerometer.setNormalizeCorrectionXYZ(&min_values, &max_values);

	// Select INT 1 for get activities
	accelerometer.useInterrupt(ADXL345_INT1);


}

void Sensor_management::update_data() {
	static CyclicTimer every500ms=CyclicTimer(500);
	static CyclicTimer every50ms=CyclicTimer(50);

	if (every500ms.watch())
	{
		//Refresh luminosity
	}

	if (every50ms.watch())
	{
		//Refresh  accelerometer
		//TODO Battery status

		Vector norm = accelerometer.readNormalizeCorrected();

		//Z orientation
		if( (orientationZ==e_orientationZ_head_up) && (norm.ZAxis< (-1.0f*VALUE_ON_G_TO_SWITCH_Z_ORIENTATION) )) {
			orientationZ=e_orientationZ_head_down;
		} else if( (orientationZ==e_orientationZ_head_down) && (norm.ZAxis> VALUE_ON_G_TO_SWITCH_Z_ORIENTATION )) {
			orientationZ=e_orientationZ_head_up;
		}

		//Y orientation
		float normYCorrected = (orientationZ==e_orientationZ_head_up)?norm.YAxis: (norm.YAxis*-1.0f); //depending of Z orientation
		if( normYCorrected< (-1.0f*VALUE_ON_G_TO_SWITCH_Y_ORIENTATION) ) {
			orientationY=e_orientationY_on_left;
		} else if( normYCorrected>VALUE_ON_G_TO_SWITCH_Y_ORIENTATION ) {
			orientationY=e_orientationY_on_right;
		} else {
			orientationY=e_orientationY_none;
		}
	}
}

int Sensor_management::get_tilt_sensor_shakes(){

	// Read activities
	Activites activ = accelerometer.readActivites();

	if (activ.isDoubleTap)
	{
		return 2;
	} else if(activ.isTap)
	{
		return 1;
	} else {
		return 0;
	}
}

int Sensor_management::get_luminosity(){
	return (100);
}

bool Sensor_management::get_battery_use(){
	return (false);
}

e_orientationZ Sensor_management::get_Zorientation(){
	return orientationZ;
}
e_orientationY Sensor_management::get_Yorientation(){
	return orientationY;
}

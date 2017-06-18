#ifndef TILT_SENSOR_H
#define TILT_SENSOR_H

/*
 * TiltSensor.h - TiltSensor library
 * J. Parisot
*/

#define SET_DOWN_TIME 100
#define MAX_TIME_BETWEEN_COUNTS	400

#define TRIGGERING_VALUE	LOW

//#define VERBOSE

class TiltSensor
{
	public:
		TiltSensor();
		boolean TiltSensor::watch(int in_value, int *count_OUT);
	private:
		boolean has_been_triggered;
		boolean i_am_still_counting_tilts;
		long time_when_first_tilted;
		long time_when_last_tilted;
		int cnt_impulses;

};

#endif

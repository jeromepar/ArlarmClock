// Do not remove the include below
#include "AlarmClock.h"
#include "sensormanagement.h"
#include "ledManagement.h"
#include "time_esp8266.h"
#include "tools.h"

static Sensor_management sensor_mngt;
static TimeManagement time;
static LEDs_management leds;
static Alarm_management alarm;

void setup()
{
	//init of sensors reading
	sensor_mngt.update_data();

	//init of time (Monday 0h0)
	time.setTime(0);

}

void loop()
{


	/********************/
	/* Cyclic functions */
	/********************/
	sensor_mngt.update_data();

	leds.update(
			sensor_mngt.get_luminosity(),
			sensor_mngt.get_battery_use(),
			false /* TODO wifi to be implemented */,
			alarm.isActive());

	//update
	update_time_if_necessary(&time);

	//TODO display time

	//alarm management with time updated
	alarm.watch(time.getTime());

}

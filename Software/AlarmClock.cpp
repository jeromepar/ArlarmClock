// Do not remove the include below
#include "AlarmClock.h"
#include "sensormanagement.h"

static Sensor_management sensor_mngt;

void setup()
{
	//init of sensors reading
	sensor_mngt.update_data();
}

void loop()
{


	/********************/
	/* Cyclic functions */
	/********************/
	sensor_mngt.update_data();
}

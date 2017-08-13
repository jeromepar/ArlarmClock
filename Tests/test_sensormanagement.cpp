/*
 * testu.cpp
 *
 *  Created on: 25 juin 2017
 *      Author: jj
 */

#include "Arduino.h"
#include "catch.hpp"
#include "../Software/sensormanagement.h"

SCENARIO("basic calls and sensor updates, at different times","[sensor_mgmt]"){
	GIVEN("initialisation"){
		Sensor_management sensor_mgmt;

		//init values
		stub_digitalset(TILT_SENSOR_PIN, TILT_TRIGGERING_VALUE); //tilt to triggered

		"nothing is done";
		REQUIRE(sensor_mgmt.get_battery_use() == false);
		REQUIRE(sensor_mgmt.get_luminosity() == 100);
		REQUIRE(sensor_mgmt.get_tilt_sensor_shakes() == 0);



		"1 sec passed  without polling";
		stub_millis(1000);
		REQUIRE(sensor_mgmt.get_battery_use() == false);
		REQUIRE(sensor_mgmt.get_luminosity() == 100);
		REQUIRE(sensor_mgmt.get_tilt_sensor_shakes() == 0);



		"polling once";
		sensor_mgmt.update_data();
		REQUIRE(sensor_mgmt.get_tilt_sensor_shakes() == 0);



		"polling twice after another wait (1s)";
		stub_digitalset(TILT_SENSOR_PIN,(TILT_TRIGGERING_VALUE+1)%2);

		//once to stop the tilt sensor
		stub_millis(1000);
		sensor_mgmt.update_data();
		//twice to stop counting
		stub_millis(1000);
		sensor_mgmt.update_data();

		REQUIRE(sensor_mgmt.get_tilt_sensor_shakes() == 1);



		"polling right again";
		REQUIRE(sensor_mgmt.get_tilt_sensor_shakes() == 0);

	}


}

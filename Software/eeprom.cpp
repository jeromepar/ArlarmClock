/*
 * eeprom.c++
 *
 *  Created on: 14 août 2017
 *      Author: Jerome
 */

#include "eeprom.h"

void get_data_from_eeprom(t_struct_in_eeprom *struct_in_eeprom, Alarm_management *alarm, myWifiForTime *wifi){

	VERBOSE(Serial.println("Accessing EEPROM..."));
	EEPROM.begin(SIZE_OF_STRUCT_IN_EEPROM);
	EEPROM.get(0x0,*struct_in_eeprom); /* first adress */

	wifi->setCredentials(struct_in_eeprom->wifi_ssid, struct_in_eeprom->wifi_psswd);

	if(struct_in_eeprom->non_random_string != NON_RANDOM_STRING){
		VERBOSE(Serial.println("Nothing in EEPROM - using defaults"));
		int a_h, a_m;
		alarm->getCurrentAlarm(&a_h, &a_m);
		struct_in_eeprom->alarm_hours = a_h;
		struct_in_eeprom->alarm_hours = a_m;

		wifi->setCredentialsDefault();

		VERBOSE(Serial.println("Force ad_hoc WIFI"));
		wifi->force_ad_hoc();

	} else {
		VERBOSE(Serial.print("Values stored in EEPROM:\nMemorized Alarm - "));
		VERBOSE(Serial.print(struct_in_eeprom->alarm_hours));
		VERBOSE(Serial.print(":"));
		VERBOSE(Serial.print(struct_in_eeprom->alarm_minutes));

		VERBOSE(Serial.print("\nCurrent alarm forced\n"));
		alarm->setCurrentAlarm(struct_in_eeprom->alarm_hours, struct_in_eeprom->alarm_minutes);

		if(struct_in_eeprom->valid_wifi_credentials) {
			VERBOSE(Serial.print("Wifi SSID: "));
			VERBOSE(Serial.print(struct_in_eeprom->wifi_ssid));
			VERBOSE(Serial.print("\nWifi PWD: "));
			VERBOSE(Serial.print(struct_in_eeprom->wifi_psswd));
			VERBOSE(Serial.print("\n"));
		} else {
			VERBOSE(Serial.print("No valid credentials found"));
			VERBOSE(Serial.println("Force ad_hoc WIFI"));
			wifi->setCredentialsDefault();
			wifi->force_ad_hoc();
		}

		VERBOSE(Serial.print("Predef Alarms: "));
		for(int i = 0; i< NBR_OF_PREDEFINED_ALARMS; i++){
			VERBOSE(Serial.print("- "));
			alarm->setPredefined_alarm_h(struct_in_eeprom->predef_alarms[i].hours,i);
			alarm->setPredefined_alarm_m(struct_in_eeprom->predef_alarms[i].minutes,i);
			VERBOSE(Serial.print(struct_in_eeprom->predef_alarms[i].hours));
			VERBOSE(Serial.print(":"));
			VERBOSE(Serial.print(struct_in_eeprom->predef_alarms[i].minutes));
		}
		VERBOSE(Serial.print("\n"));
	}

}

void send_data_to_eeprom(t_struct_in_eeprom *struct_in_eeprom, Alarm_management *alarm, bool with_valid_credentials){

	VERBOSE(Serial.println("updating EEPROM..."));
	EEPROM.begin(SIZE_OF_STRUCT_IN_EEPROM);

	if(struct_in_eeprom->non_random_string != NON_RANDOM_STRING){
		struct_in_eeprom->non_random_string = NON_RANDOM_STRING;
		struct_in_eeprom->valid_wifi_credentials = false;
	}

	if(with_valid_credentials){
		struct_in_eeprom->valid_wifi_credentials = true;
	}

	int a_h, a_m;
	alarm->getCurrentAlarm(&a_h, &a_m);
	struct_in_eeprom->alarm_hours = a_h;
	struct_in_eeprom->alarm_minutes = a_m;

	for(int i = 0; i< NBR_OF_PREDEFINED_ALARMS; i++){
		int h,m;
		alarm->getNextAlarm(&h, &m, (i==0));
		struct_in_eeprom->predef_alarms[i].hours=h;
		struct_in_eeprom->predef_alarms[i].minutes=m;
	}


	EEPROM.put(0x0,*struct_in_eeprom); /* first adress */
	EEPROM.commit();
}

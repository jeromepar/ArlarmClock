/*
 * eeprom.h
 *
 *  Created on: 14 août 2017
 *      Author: Jerome
 */

#ifndef EEPROM_H_
#define EEPROM_H_

#include <eeprom.h>
#include "config.h"
#include "Alarmmanagement.h" //for NBR_OF_PREDEFINED_ALARMS && Alarm_management
#include "myWifiForTime.h" // for myWifiForTime

typedef struct {
	long non_random_string; /* valorized if something is stored */
	int alarm_hours;
	int alarm_minutes;
	bool valid_wifi_credentials;
	char wifi_ssid[CONST_SSID_SIZE];
	char wifi_psswd[CONST_PWD_SIZE];
	s_alarm predef_alarms[NBR_OF_PREDEFINED_ALARMS];
} t_struct_in_eeprom;

#define SIZE_OF_STRUCT_IN_EEPROM sizeof(t_struct_in_eeprom)
#define NON_RANDOM_STRING	0xA5A6


void get_data_from_eeprom(t_struct_in_eeprom *struct_in_eeprom, Alarm_management *alarm, myWifiForTime *wifi);
void send_data_to_eeprom(t_struct_in_eeprom *struct_in_eeprom, Alarm_management *alarm, bool with_valid_credentials);

#endif /* EEPROM_H_ */

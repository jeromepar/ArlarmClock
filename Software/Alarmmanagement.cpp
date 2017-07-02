/*
 * Alarmmanagement.cpp
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */

#include "Alarmmanagement.h"

Alarm_management::Alarm_management() {
	disable();
	isringing = false;

	alarm_manual = predefined_alarms[0];
	current_alarm = alarm_manual;

	setCurrent_alarm();


}

void Alarm_management::watch(long currentTime_in_s) {
	//process to do only if activated
	if (alarm.getState() && !isringing)
	{
		if (alarm.is_ringing(currentTime_in_s))
		{
			isringing = true;
		}
	}

	if(isringing)
	{
		//TODO Launch DAC if not already done
		//TODO disable if manual action (snooze / turning ti off)
	}
}


bool Alarm_management::isActive() {
	return alarm.getState();
}

void Alarm_management::setCurrent_alarm(){
	alarm.setValue(current_alarm.hours, current_alarm.minutes);
}

void Alarm_management::enable(){
	alarm.enable();
}

void Alarm_management::disable(){
	alarm.disable();
}

void Alarm_management::getCurrentAlarm(int *hours, int *minutes){
	*hours = current_alarm.hours;
	*minutes = current_alarm.minutes;
}

void Alarm_management::getNextAlarm(int *hours, int *minutes, bool from_start){
	static int i=0;
	if(from_start){
		i=0;
	}

	*hours=predefined_alarms[i].hours;
	*minutes=predefined_alarms[i].minutes;

	i=(i+1)%NBR_OF_PREDEFINED_ALARMS;

}
void Alarm_management::setCurrentAlarm(int hours, int minutes){
	current_alarm.hours = hours;
	current_alarm.minutes = minutes;
	setCurrent_alarm();
}

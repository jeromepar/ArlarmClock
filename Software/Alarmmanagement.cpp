/*
 * Alarmmanagement.cpp
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */

#include "Alarmmanagement.h"
#include "config.h"

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
		if (alarm.is_ringing(currentTime_in_s,MAX_TIME_TO_RING))
		{
			isringing = true;
		}
	}

	if(isringing)
	{
		VERBOSE(Serial.println("Is Ringing!"));
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

void Alarm_management::setPredefined_alarm_h(int hours, int num){
	(num>=NBR_OF_PREDEFINED_ALARMS) ? num=NBR_OF_PREDEFINED_ALARMS-1 : num ;
	(hours>23)?hours=0:hours;
	predefined_alarms[num].hours = hours;
}
void Alarm_management::setPredefined_alarm_m(int min, int num){
	(num>=NBR_OF_PREDEFINED_ALARMS) ? num=NBR_OF_PREDEFINED_ALARMS-1 : num ;
	(min>59)?min=0:min;
	predefined_alarms[num].minutes = min;
}

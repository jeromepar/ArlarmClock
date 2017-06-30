/*
 * Alarmmanagement.h
 *
 *  Created on: 30 juin 2017
 *      Author: jj
 */

#ifndef ALARMMANAGEMENT_H_
#define ALARMMANAGEMENT_H_

#include "Alarm.h"


#define MIN2SEC 60
#define HOUR2SEC (60*MIN2SEC)

typedef struct {
	int hours;
	int minutes;
} s_alarm;

static const s_alarm predefined_alarms[5]={
		{07,00},
		{07,15},
		{07,30},
		{8,00},
		{9,00}
};
#define NBR_OF_PREDEFINED_ALARMS	5


class Alarm_management {
private:
	Alarm alarm;
	s_alarm alarm_manual;
	s_alarm current_alarm;
	bool isringing;

	void setCurrent_alarm();

public:
	Alarm_management();
	bool isActive();
	void enable();
	void disable();

	void watch(long currentTime_in_s);
};

#endif /* ALARMMANAGEMENT_H_ */
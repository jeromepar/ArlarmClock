/*
 * time_esp8266.cpp
 *
 *  Created on: 29 juin 2017
 *      Author: jj
 */

#include "time_esp8266.h"

CyclicTimer every1h = CyclicTimer(60*60*1000);

void update_time_if_necessary(TimeManagement *time){
	if (every1h.watch())
	{
		//update time through wifi
		//& print on RS how much micros we lost
	}


}

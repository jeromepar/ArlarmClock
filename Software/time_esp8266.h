/*
 * time_esp8266.h
 *
 *  Created on: 29 juin 2017
 *      Author: jj
 */

#ifndef TIME_ESP8266_H_
#define TIME_ESP8266_H_

#include "TimeManagement.h"
#include "CyclicTimer.h"
#include "myWifiForTime.h"

void update_time_if_necessary(TimeManagement *time, myWifiForTime *wifi);


#endif /* TIME_ESP8266_H_ */

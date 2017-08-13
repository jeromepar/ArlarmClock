/*
 * Arduino.h
 *
 *  Created on: 27 juin 2017
 *      Author: jj
 */

#ifndef ARDUINO_H_
#define ARDUINO_H_

#define ARDUINO 101
typedef bool boolean;
#define LOW 0
#define HIGH 0


int millis();
long micros();
void stub_millis(int ms);

bool digitalRead(int pin);
void stub_digitalset(int pin, bool state);


#endif /* ARDUINO_H_ */

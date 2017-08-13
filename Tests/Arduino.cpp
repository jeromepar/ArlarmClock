#include "Arduino.h"


static int millisec=0;
int millis(){
	return millisec;
}
long micros(){
	return millisec*1000;
}
void stub_millis(int ms){
	millisec+=ms;
}

static bool pinDigitalState[20];
bool digitalRead(int pin){
	return pinDigitalState[pin];
}
void stub_digitalset(int pin, bool state){
	pinDigitalState[pin]=state;
}

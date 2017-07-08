// Import required libraries
#include "myWifiForTime.h"


myWifiForTime wifiTime;

void setup(void)
{
	// Start Serial
	Serial.begin(115200);
	delay(50);
	Serial.println();
	Serial.println("initializing");

	wifiTime.init();
	wifiTime.requestTime();
}


void loop() {
	wifiTime.update();
	e_state_time_request s =wifiTime.getState();
	//Serial.println(s);

	if (s==e_state_time_request_time_available){
		Serial.print("seconds from Monday 0h0 : ");
		Serial.println(wifiTime.getTimeReceived());
		delay(10000);
		wifiTime.requestTime();
	}

	delay(50);

}

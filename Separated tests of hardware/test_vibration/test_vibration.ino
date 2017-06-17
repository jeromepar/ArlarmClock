#include "Arduino.h"

int shockPin = 5; // Use Pin 5 as our Input

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	pinMode (shockPin, INPUT) ;
}

static boolean memo_triggered=false;
static long timer_ref;
static long timer;

// The loop function is called in an endless loop
void loop()
{
	boolean triggered = ! digitalRead (shockPin) ; // read the value from our sensor

	if(triggered)
	{
		timer = millis();
		if(memo_triggered == false) {
			memo_triggered = true;
			timer_ref = millis();
		}
	} else if (memo_triggered && (millis()-timer>100)) {
		memo_triggered = false;
		Serial.print("Off :");
		Serial.println(millis()-timer_ref);
	}

}

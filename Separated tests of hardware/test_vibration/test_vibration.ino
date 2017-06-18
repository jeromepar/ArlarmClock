#include "Arduino.h"
#include "TiltSensor.h"

int shockPin = 5; // Use Pin 5 as our Input

TiltSensor tilt_sensor;

//The setup function is called once at startup of the sketch
void setup()
{
	Serial.begin(9600);
	pinMode (shockPin, INPUT) ;
}



// The loop function is called in an endless loop
void loop()
{
	int count;
	boolean result = tilt_sensor.watch(digitalRead(shockPin),&count);

	if(result)
	{
		Serial.print("Result: ");
		Serial.println(count);
	}

}

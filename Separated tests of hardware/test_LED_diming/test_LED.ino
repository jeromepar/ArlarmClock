#define PIN_OUT  5

#define MANUAL

static unsigned char hash[255];

void setup()
{
	Serial.begin(9600);
	pinMode(PIN_OUT,OUTPUT);

	for(int i = 0; i<256; i++){
		hash[i]=(unsigned char)(255.0*pow((i+50)/305.0,3.0));
	}
}


void loop()
{

#ifdef MANUAL
	static int i=0;
	 analogWrite(PIN_OUT,hash[i]);
	 delay(30);
	 i=(i+1)%256;

#else
	int valA;
	if (Serial.available()>0)
	  {
		String s=Serial.readString();
	    valA=s.toInt();
	    Serial.print("value set to: ");
	    Serial.println(valA);
	    analogWrite(PIN_OUT,valA);
	  }
#endif
}

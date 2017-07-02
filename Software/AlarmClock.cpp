// Do not remove the include below
#include "AlarmClock.h"
#include "sensormanagement.h"
#include "ledManagement.h"
#include "time_esp8266.h"
#include "Alarmmanagement.h"
#include "TM1637Display.h"
#include "timer.h"
#include "tools.h"

static Sensor_management sensor_mngt;
static TimeManagement time;
static LEDs_management leds;
static Alarm_management alarm;
static TM1637Display AffSeg(AFFSEG_PIN_CLK, AFFSEG_PIN_DIO);  //set up the 4-Digit Display.

void setup()
{
	VERBOSE(Serial.begin(19200));
	delay(20);
	VERBOSE(Serial.println("Initializing..."));

	//init of sensors reading
	sensor_mngt.update_data();

	//set the display to maximum brightness
	AffSeg.setBrightness(0x07);

	//init of time (Monday 12h10)
	time.setTime(12*60*60+10*60);

	VERBOSE(Serial.println("Init done!"));

}

typedef enum  {
	e_UI_default, //default : time display
	e_UI_alarm_display,	//display current alarm
	e_UI_alarm_selection, //cycle/program alarm
	e_UI_MAX
} e_state_UI;

void do_nothing(){}
Timer timer_in_state(&do_nothing);


void UI(Sensor_management * sensor_mngt, Alarm_management * alarm, TimeManagement *time, TM1637Display *disp){

	static e_state_UI state = e_UI_default;
	static bool first_entry_in_state = true;

	bool update_display = false;
	static int luminosity=0;


	int tilt_sensor_shakes = sensor_mngt->get_tilt_sensor_shakes();

#ifdef VERBOSE_ACTIVATED
	if(tilt_sensor_shakes>0)
	{
		Serial.print("Tilts: ");
		Serial.println(tilt_sensor_shakes);
	}
#endif


	/* the display is updated if luminosity change (every 500ms max) */
	{
		static CyclicTimer every500ms=CyclicTimer(500);
		if (every500ms.watch())
		{
			int current_lum = sensor_mngt->get_luminosity();
			if( current_lum != luminosity){
				update_display = true;
				luminosity = current_lum;
			}
		}
	}

	switch(state){

	//Time display
	case e_UI_default:
	{
		static CyclicTimer every500ms=CyclicTimer(500);
		static int minutes = 0; //static because we want to compare it between 2 calls
		static int hours = 0;

		/* check if time has change every 500ms -> we only display the minutes */
		if (every500ms.watch())
		{
			int current_min = time->minute();
			if( current_min != minutes){
				update_display = true;
				minutes = current_min;
				hours = time->hour();
			}
		}

		if(first_entry_in_state || update_display){
			VERBOSE(Serial.print("display time :"));
			VERBOSE(Serial.print(hours*100+minutes));
			VERBOSE(Serial.print(" lum:"));
			VERBOSE(Serial.println((luminosity*7)/100));
			//TODO : use the accelerometer to reverse the display

			//use hours & minutes & luminosity
			disp->showNumberDec(hours*100+minutes, true /*upside_down*/);
			disp->setBrightness((luminosity*7)/100);

			//TODO : delimiter between hours & minutes

			first_entry_in_state = false;
		}

		if(tilt_sensor_shakes==NBR_OF_TILTS_TO_WAKE_UP){
			VERBOSE(Serial.println("Going to state e_UI_alarm_display"));
			state = e_UI_alarm_display;
			first_entry_in_state = true;
		} else if (tilt_sensor_shakes==NBR_OF_TILTS_TO_SWITCH_TO_ALARM_SELECTION)
		{
			VERBOSE(Serial.println("Going to state e_UI_alarm_display"));
			state = e_UI_alarm_selection;
			first_entry_in_state = true;
		}
	}
	break;

	//current alarm time display
	case e_UI_alarm_display:
	{
		static int hours = 0;
		static int minutes = 0;

		if(first_entry_in_state){
			timer_in_state.start(MAX_TIME_SHOWING_CURRENT_ALARM);
			alarm->getCurrentAlarm(&hours,&minutes);
			VERBOSE(Serial.println("Timer started & Alarm requested"));
		}

		if(first_entry_in_state || update_display){

			VERBOSE(Serial.print("Current alarm :"));
			VERBOSE(Serial.print(hours*100+minutes));
			VERBOSE(Serial.print(" lum:"));
			VERBOSE(Serial.println((luminosity*7)/100));
			//TODO : use the accelerometer to reverse the display

			//use hours & minutes & luminosity
			disp->showNumberDec(hours*100+minutes, true /*upside_down*/);
			disp->setBrightness((luminosity*7)/100);

			//TODO : delimiter between hours & minutes

			first_entry_in_state = false;
		}

		/* If timer expired : leave state */
		if (timer_in_state.watch())
		{
			VERBOSE(Serial.println("Going to state e_UI_default"));
			state = e_UI_default;
			first_entry_in_state = true;
		} else if (tilt_sensor_shakes == NBR_OF_TILTS_TO_SWITCH_TO_ALARM_MANAGEMENT) {
			timer_in_state.stop();
			VERBOSE(Serial.println("Going to state e_UI_alarm_selection"));
			state = e_UI_alarm_selection;
			first_entry_in_state = true;
		}


	}
	break;

	//program/cycle to set custom alarm time
	case e_UI_alarm_selection:
	{
		static int hours = 0;
		static int minutes = 0;

		if(first_entry_in_state){
			timer_in_state.start(MAX_TIME_SHOWING_ALARM_SELECTION);
			alarm->getNextAlarm(&hours,&minutes,true /* from start */);
			VERBOSE(Serial.println("Timer started & Next Alarm requested"));
		} else if (tilt_sensor_shakes==1)
		{
			//restart timer (because of the new input)
			timer_in_state.start(MAX_TIME_SHOWING_ALARM_SELECTION);
			//obtain next value
			alarm->getNextAlarm(&hours,&minutes);
			//display new value
			update_display = true;
		}
		if(first_entry_in_state || update_display){

			VERBOSE(Serial.print("Current selected alarm :"));
			VERBOSE(Serial.print(hours*100+minutes));
			VERBOSE(Serial.print(" lum:"));
			VERBOSE(Serial.println((luminosity*7)/100));
			//TODO : use the accelerometer to reverse the display

			//use hours & minutes & luminosity
			disp->showNumberDec(hours*100+minutes, true /*upside_down*/);
			disp->setBrightness((luminosity*7)/100);

			//TODO : delimiter between hours & minutes

			first_entry_in_state = false;
		}

		/* If timer expired : leave state */
		if (timer_in_state.watch())
		{
			VERBOSE(Serial.println("Setting Alarm no the new value"));
			alarm->setCurrentAlarm(hours, minutes);
			VERBOSE(Serial.println("Going to state e_UI_default"));
			state = e_UI_default;
			first_entry_in_state = true;
		}

	}
	break;

	case e_UI_MAX:
		/* FALLTHROUGH */
	default:
		ERROR(__FILE__,__LINE__);
		break;
	}

}

void loop()
{


	/********************/
	/* Cyclic functions */
	/********************/
	sensor_mngt.update_data();

	leds.update(
			sensor_mngt.get_luminosity(),
			sensor_mngt.get_battery_use(),
			false /* TODO wifi to be implemented */,
			alarm.isActive());

	//update
	update_time_if_necessary(&time);

	//TODO display time

	//UserInterface
	UI(&sensor_mngt,&alarm,&time,&AffSeg);

	//alarm management with time updated
	alarm.watch(time.getTime());

}

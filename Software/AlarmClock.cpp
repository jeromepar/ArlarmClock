// Do not remove the include below
#include "AlarmClock.h"
#include "sensormanagement.h"
#include "ledManagement.h"
#include "time_esp8266.h"
#include "Alarmmanagement.h"
#include "TM1637Display.h"
#include "timer.h"
#include "tools.h"
#include "myWifiForTime.h"

static Sensor_management sensor_mngt;
static TimeManagement time;
static LEDs_management leds;
static Alarm_management alarm;
static myWifiForTime wifiTime;
static TM1637Display AffSeg(AFFSEG_PIN_CLK, AFFSEG_PIN_DIO);  //set up the 4-Digit Display.

void setup()
{
	VERBOSE(Serial.begin(115200));
	delay(20);
	VERBOSE(Serial.println("Initializing..."));

	//init of sensors reading
	sensor_mngt.init();
	sensor_mngt.update_data();

	//init for the wifi
	wifiTime.init();
	VERBOSE(Serial.println("time_requested"));
	wifiTime.requestTime(); //at boot, get time

	//set the display to maximum brightness
	AffSeg.setBrightness(0x07);

	//init of time (Monday 00h00)
	time.setTime(0);

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

//maximum delta +-59mn
void addInterval(int delta_mn, int *hours, int *minutes){
	*minutes+=delta_mn;
	if (*minutes >= 60){
		*minutes-=60;
		*hours=(*hours+1)%24;
	} else if (*minutes < 0) {
		*minutes+=60;
		//*hours=(*hours-1)%24; -> Not working with negative values
		if ((*hours-1)<0){
			*hours+=23;
		} else {
			*hours-=1;
		}
	}
}

void UI(Sensor_management * sensor_mngt, Alarm_management * alarm, TimeManagement *time, TM1637Display *disp){

	static e_state_UI state = e_UI_default;
	static bool first_entry_in_state = true;

	bool update_display = false;
	static int luminosity=0;
	static e_orientationZ orien_Z=e_orientationZ_head_up;


	int tilt_sensor_shakes = sensor_mngt->get_tilt_sensor_shakes();
	e_orientationY Yorientation = sensor_mngt->get_Yorientation();

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

	/* the display is updated if the orientation change */
	{

		e_orientationZ current_orientation = sensor_mngt->get_Zorientation();
		if( current_orientation != orien_Z){
			update_display = true;
			orien_Z = current_orientation;
		}
	}

	switch(state){

	//Time display
	case e_UI_default:
	{
		static CyclicTimer every500ms=CyclicTimer(500);
		static int minutes = 0; //static because we want to compare it between 2 calls
		static int hours = 0;   //static because we want to compare it between 2 calls

		/* check if time has change every 500ms -> we only display the minutes */
		if (every500ms.watch())
		{
			static bool first_change = true;
			int current_min = time->minute();
			int current_hour = time->hour();

			if( current_min != minutes){

				update_display = true;
				minutes = current_min;

				//we don't want to trigger an update on the init (time is changed there)
				if( (!first_change) && (hours != current_hour) )
				{
					//DEBUG1: commented in order to examine the error 1 wifiTime.requestTime();
					VERBOSE(Serial.println("time_requested"));
				}
				hours = current_hour;
				first_change = false;
			}
		}

		if(first_entry_in_state || update_display){
			VERBOSE(Serial.print("display time :"));
			VERBOSE(Serial.print(hours*100+minutes));
			VERBOSE(Serial.print(" lum:"));
			VERBOSE(Serial.println((luminosity*7)/100));

			//use hours & minutes & luminosity
			disp->showNumberDec(hours*100+minutes, (orien_Z==e_orientationZ_head_down));
			disp->setBrightness((luminosity*7)/100);

			//TODO : delimiter between hours & minutes

			first_entry_in_state = false;
		}

		if(tilt_sensor_shakes==NBR_OF_TILTS_TO_WAKE_UP){
			VERBOSE(Serial.println("Going to state e_UI_alarm_display"));
			state = e_UI_alarm_display;
			first_entry_in_state = true;
		} else if (tilt_sensor_shakes==NBR_OF_TILTS_TO_WAKE_UP_TO_ALARM_MANAGEMENT) {
			VERBOSE(Serial.println("Going to state e_UI_alarm_selection"));
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

			//use hours & minutes & luminosity
			disp->showNumberDec(hours*100+minutes, (orien_Z==e_orientationZ_head_down));
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
		static Timer timer_between_imputs(&do_nothing);

		if(first_entry_in_state){
			timer_in_state.start(MAX_TIME_SHOWING_ALARM_SELECTION);
			alarm->getNextAlarm(&hours,&minutes,true /* from start */);
			VERBOSE(Serial.println("Timer started & Next Alarm requested"));

		} else if ((Yorientation == e_orientationY_on_left) && (!timer_between_imputs.is_active()) ) {
			VERBOSE(Serial.println("Orientation Left : -15mn"));
			addInterval(-15,&hours, &minutes);
			update_display = true;
			timer_between_imputs.start(MIN_TIME_BETWEEN_INPUTS);
			//restart timer (because of the new input)
			timer_in_state.start(MAX_TIME_SHOWING_ALARM_SELECTION);

		} else if ( (Yorientation == e_orientationY_on_right) && (!timer_between_imputs.is_active())) {
			VERBOSE(Serial.println("Orientation Left : +15mn"));
			addInterval(+15,&hours, &minutes);
			update_display = true;
			timer_between_imputs.start(MIN_TIME_BETWEEN_INPUTS);
			//restart timer (because of the new input)
			timer_in_state.start(MAX_TIME_SHOWING_ALARM_SELECTION);

		} else if ( (tilt_sensor_shakes==2) && (!timer_between_imputs.is_active())) {
			timer_between_imputs.start(MIN_TIME_BETWEEN_INPUTS);
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

			//use hours & minutes & luminosity
			disp->showNumberDec(hours*100+minutes, (orien_Z==e_orientationZ_head_down));
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

	/* update alarm status according to the orientation */
	(sensor_mngt.get_Zorientation()==e_orientationZ_head_up)?alarm.disable():alarm.enable();



	//update
	update_time_if_necessary(&time);

	//UserInterface
	UI(&sensor_mngt,&alarm,&time,&AffSeg);

	//alarm management with time updated
	alarm.watch(time.getTime());

	leds.update(
			sensor_mngt.get_luminosity(),
			sensor_mngt.get_battery_use(),
			(wifiTime.getState()!=e_state_time_request_idle),
			alarm.isActive());

	//wifi update
	wifiTime.update();
	if (wifiTime.getState()==e_state_time_request_time_available){
		unsigned long error = time.setTime(wifiTime.getTimeReceived());
#ifdef VERBOSE_ACTIVATED
		Serial.print("time_updated, error on time (s) : ");
		Serial.println(error);
#endif
	}

}

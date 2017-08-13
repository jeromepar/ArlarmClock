/*
 * myWifiForTime.cpp
 *
 *  Created on: 8 juil. 2017
 *      Author: Jerome
 */

#include "myWifiForTime.h"
#include "timer.h"


IPAddress    apIP_hotspot(42,42,42,42);  // Defining a static IP address: local & gateway
DNSServer dnsServer;

// Define a web server at port 80 for HTTP
ESP8266WebServer server(80);

myWifiForTime *ptr_for_handler;

void handleSubmit(){
	VERBOSE(Serial.println("Entering submit handle"));
	if (server.args() > 0 ) {
		for ( uint8_t i = 0; i < server.args(); i++ ) {
			if(server.argName(i) == "SSID"){
				server.arg(i).toCharArray(ptr_for_handler->get_ssid(), CONST_SSID_SIZE, 0);
				VERBOSE(Serial.print("recorded SSID: "));
				VERBOSE(Serial.print(ptr_for_handler->get_ssid()));
				VERBOSE(Serial.print("\n"));
			} else if(server.argName(i) == "password"){
				server.arg(i).toCharArray(ptr_for_handler->get_password(), CONST_PWD_SIZE, 0);
				VERBOSE(Serial.print("recorded pwd: "));
				VERBOSE(Serial.print(ptr_for_handler->get_password()));
				VERBOSE(Serial.print("\n"));
			} else if(server.argName(i) == "alarm1h"){
				ptr_for_handler->alarms->setPredefined_alarm_h(server.arg(i).toInt(), 0);
			} else if(server.argName(i) == "alarm1m"){
				ptr_for_handler->alarms->setPredefined_alarm_m(server.arg(i).toInt(), 0);
			} else if(server.argName(i) == "alarm2h"){
				ptr_for_handler->alarms->setPredefined_alarm_h(server.arg(i).toInt(), 1);
			} else if(server.argName(i) == "alarm2m"){
				ptr_for_handler->alarms->setPredefined_alarm_m(server.arg(i).toInt(), 1);
			} else if(server.argName(i) == "alarm3h"){
				ptr_for_handler->alarms->setPredefined_alarm_h(server.arg(i).toInt(), 2);
			} else if(server.argName(i) == "alarm3m"){
				ptr_for_handler->alarms->setPredefined_alarm_m(server.arg(i).toInt(), 2);
			} else if(server.argName(i) == "alarm4h"){
				ptr_for_handler->alarms->setPredefined_alarm_h(server.arg(i).toInt(), 3);
			} else if(server.argName(i) == "alarm4m"){
				ptr_for_handler->alarms->setPredefined_alarm_m(server.arg(i).toInt(), 3);
			} else if(server.argName(i) == "alarm5h"){
				ptr_for_handler->alarms->setPredefined_alarm_h(server.arg(i).toInt(), 4);
			} else if(server.argName(i) == "alarm5m"){
				ptr_for_handler->alarms->setPredefined_alarm_m(server.arg(i).toInt(), 4);
			}
		}
	}

	server.send ( 200, "text/html", "Values updated, restarting..." );
	ptr_for_handler->end_configuration();
}

// Default IP in AP mode is 192.168.4.1
void handleRoot() {




	static char html[2000];

	int sec = millis() / 1000;
	int min = sec / 60;
	int hr = min / 60;

	int hc,mc,h1, m1, h2, m2, h3, m3, h4, m4, h5, m5;
	ptr_for_handler->alarms->getCurrentAlarm(&hc,&mc);
	ptr_for_handler->alarms->getNextAlarm(&h1,&m1, true);
	ptr_for_handler->alarms->getNextAlarm(&h2,&m2, false);
	ptr_for_handler->alarms->getNextAlarm(&h3,&m3, false);
	ptr_for_handler->alarms->getNextAlarm(&h4,&m4, false);
	ptr_for_handler->alarms->getNextAlarm(&h5,&m5, false);

	// Build an HTML page to display on the web-server root address
	sprintf ( html,

			"<html>\
<head>\
<title>WiFi Alarm config</title>\
<style>\
body { background-color: #cccccc; Color: #000000; }\
h1 { Color: #AA0000; }\
</style>\
</head>\
<body>\
<h1>ESP8266 WiFi Network configuration</h1>\
<p>Uptime: %02d:%02d:%02d</p>\
<form action='http://42.42.42.42/submit' method='POST'>\
WiFi SSID: <input name='SSID' value=\"%s\" maxlength='32' size='40'><br>\
WiFi PWD:  <input name='password' value=\"%s\" maxlength='64' size='80'><br>\
<p>Current memorized alarm: <b>%02d:%02d</b><p>\
\
<p>Predefined Alarms:<p>\
<p>1 - <input name='alarm1h' value=\"%02d\" maxlength='2' size='2'>:<input name='alarm1m' value=\"%02d\" maxlength='2' size='2'><p>\
<p>2 - <input name='alarm2h' value=\"%02d\" maxlength='2' size='2'>:<input name='alarm2m' value=\"%02d\" maxlength='2' size='2'><p>\
<p>3 - <input name='alarm3h' value=\"%02d\" maxlength='2' size='2'>:<input name='alarm3m' value=\"%02d\" maxlength='2' size='2'><p>\
<p>4 - <input name='alarm4h' value=\"%02d\" maxlength='2' size='2'>:<input name='alarm4m' value=\"%02d\" maxlength='2' size='2'><p>\
<p>5 - <input name='alarm5h' value=\"%02d\" maxlength='2' size='2'>:<input name='alarm5m' value=\"%02d\" maxlength='2' size='2'><p>\
<input type='submit' value='Use this new configuration & restart ESP8266'>\
<p>Click <a href=\"javascript:window.location.reload();\">here</a> to refresh the page now.</p>\
</body>\
</html>",

hr, min % 60, sec % 60,
ptr_for_handler->get_ssid(),
ptr_for_handler->get_password(),
hc,mc,
h1,m1, h2,m2, h3,m3, h4,m4, h5,m5
	);

	server.send ( 200, "text/html", html );
}

void handleNotFound() {
	String message = "File Not Found\n\n";
	message += "URI: ";
	message += server.uri();
	message += "\nMethod: ";
	message += ( server.method() == HTTP_GET ) ? "GET" : "POST";
	message += "\nArguments: ";
	message += server.args();
	message += "\n";

	for ( uint8_t i = 0; i < server.args(); i++ ) {
		message += " " + server.argName ( i ) + ": " + server.arg ( i ) + "\n";
	}
	server.send ( 404, "text/plain", message );
}


Timer timer_in_WIFI_state(NULL);

myWifiForTime::myWifiForTime() {
	alarms=NULL;
	// TODO Auto-generated constructor stub
	Udp=NULL;
	timeServer=NULL;

	ssid = NULL;
	password = NULL;

	state = e_state_time_request_idle;
	time_requested = false;
	time_received=0;
	new_EEPROM_config = false;

	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, CONST_NTP_PACKET_SIZE);
}

void myWifiForTime::init(Alarm_management *AM){
	static WiFiUDP myUDP;
	static IPAddress mytimeServer(129, 6, 15, 28); // time.nist.gov NTP server
	Udp=&myUDP;
	timeServer=&mytimeServer;
	alarms = AM;

	//default : no wifi
	WiFi.forceSleepBegin();

	ptr_for_handler=this;
}

void myWifiForTime::update(){

	const char *ssid_hotspot = "ESP8266_config";

	switch(state){
	case e_state_time_request_idle:

		if(time_requested){
			time_requested = false;

			// Connect to WiFi
			WiFi.forceSleepWake();
			WiFi.begin(ssid, password);
			timer_in_WIFI_state.start(MAX_TIME_WAITING_FOR_WIFI_MS);
			state=e_state_time_request_wifi_connecting;

			VERBOSE(Serial.print("Wifi connecting to \""));
			VERBOSE(Serial.print(ssid));
			VERBOSE(Serial.print("\" - \""));
			VERBOSE(Serial.print(password));
			VERBOSE(Serial.print("\"\n"));
		}
		break;

	case e_state_time_request_wifi_connecting:
		if (WiFi.status() == WL_CONNECTED){
			Udp->begin(CONST_PORT_FOR_UDP_PACKET);
			sendNTPpacket(timeServer); // send an NTP packet to a time server

			state = e_state_time_request_waiting_UDP_response;

			/* If timer expired : leave state & mount adhoc wifi */
		} else if (timer_in_WIFI_state.watch()) {
			VERBOSE(Serial.println("TimeOUT on Registered WiFi connection"));
			WiFi.disconnect();
			state = e_state_time_request_WIFI_ad_hoc_start;
		}
		break;
	case e_state_time_request_waiting_UDP_response:
		if (Udp->parsePacket() != 0){
			WiFi.disconnect();
			WiFi.forceSleepBegin();

			Udp->read(packetBuffer, CONST_NTP_PACKET_SIZE); // read the packet into the buffer
			unsigned long epoch = get_epoch_time_from_buffer();
			calculate_time(epoch);

			state = e_state_time_request_time_available;
		}
		break;
	case e_state_time_request_time_available:
		//only the getTimeReceived function can exit this state
		break;

	case e_state_time_request_WIFI_ad_hoc_start:

		VERBOSE(Serial.println("AD HOC wifi started :"));
		// Launching AD-Hoc wifi for the user to connect & valorize WifiCredentials
		WiFi.forceSleepWake();
		if (WiFi.status()!=WL_DISCONNECTED) {
			WiFi.disconnect();
		}



		//set-up the custom IP address
		WiFi.mode(WIFI_OFF);
		delay(100);
		WiFi.mode(WIFI_AP_STA);
		WiFi.softAPConfig(apIP_hotspot, apIP_hotspot, IPAddress(255, 255, 255, 0));   // subnet FF FF FF 00

		VERBOSE(Serial.print("Wifi ON : \""));
		VERBOSE(Serial.print(ssid_hotspot));
		VERBOSE(Serial.print("\"\n"));

		dnsServer.start(DNS_PORT, "*", apIP_hotspot);

		/* You can remove the password parameter if you want the AP to be open. */
		WiFi.softAP(ssid_hotspot);

		VERBOSE(Serial.print("AP IP address: "));
		VERBOSE(Serial.println(WiFi.softAPIP()));

		server.on ( "/", handleRoot );
		server.on ( "/submit", handleSubmit);
		server.onNotFound ( handleNotFound );

		server.begin();
		Serial.println("HTTP server started");

		state=e_state_time_request_WIFI_ad_hoc_running;


		break;

	case e_state_time_request_WIFI_ad_hoc_running:
		dnsServer.processNextRequest();
		server.handleClient();
		break;

	case e_state_time_request_WIFI_ad_hoc_finish:
		server.close();
		dnsServer.stop();
		WiFi.disconnect();
		state = e_state_time_request_idle;
		time_requested = true;
		new_EEPROM_config = true;
		break;

	default:
		break;
	}
}

// send an NTP request to the time server at the given address
void myWifiForTime::sendNTPpacket(IPAddress * address) {

	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, CONST_NTP_PACKET_SIZE);

	// Initialize values needed to form NTP request
	packetBuffer[0] = 0b11100011;   // LI, Version, Mode
	packetBuffer[1] = 0;     // Stratum, or type of clock
	packetBuffer[2] = 6;     // Polling Interval
	packetBuffer[3] = 0xEC;  // Peer Clock Precision
	// 8 bytes of zero for Root Delay & Root Dispersion
	packetBuffer[12]  = 49;
	packetBuffer[13]  = 0x4E;
	packetBuffer[14]  = 49;
	packetBuffer[15]  = 52;

	// all NTP fields have been given values, now
	// you can send a packet requesting a timestamp:
	Udp->beginPacket(*address, 123); //NTP requests are to port 123
	Udp->write(packetBuffer, CONST_NTP_PACKET_SIZE);
	Udp->endPacket();
}

/* valid until 2099 */
int myWifiForTime::getSecondsToAdjustDstEurope(int year, int month, int day)
{
	// last sunday of march
	int beginDSTDate=  (31 - (5* year /4 + 4) % 7);
	int beginDSTMonth=3;
	//last sunday of october
	int endDSTDate= (31 - (5 * year /4 + 1) % 7);
	int endDSTMonth=10;
	// DST is valid as:
	if (((month > beginDSTMonth) && (month < endDSTMonth))
			|| ((month == beginDSTMonth) && (day >= beginDSTDate))
			|| ((month == endDSTMonth) && (day <= endDSTDate)))
		return CONST_GMT_TO_DST;  // DST europe = utc +2 hour
	else return CONST_GMT_TO_NON_DST; // nonDST europe = utc +1 hour
}

unsigned long myWifiForTime::get_epoch_time_from_buffer(){

	//the timestamp starts at byte 40 of the received packet and is four bytes,
	// or two words, long. First, extract the two words:
	unsigned long highWord = word(packetBuffer[40], packetBuffer[41]);
	unsigned long lowWord = word(packetBuffer[42], packetBuffer[43]);
	// combine the four bytes (two words) into a long integer
	// this is NTP time (seconds since Jan 1 1900):
	unsigned long secsSince1900 = highWord << 16 | lowWord;

	// now convert NTP time into everyday time:
	// Unix time starts on Jan 1 1970. In seconds, that's 2208988800:
	const unsigned long seventyYears = 2208988800UL;
	// subtract seventy years:
	unsigned long epoch = secsSince1900 - seventyYears;
	return epoch;
}

void myWifiForTime::calculate_time(unsigned long epoch_in){

	unsigned long seconds,minutes,wDay,year;

	unsigned long epoch=epoch_in; // work variable

	{
		seconds = epoch % 60;
		epoch /= 60;
		minutes = epoch % 60;
		epoch /= 60;
		//no use for hours
		epoch /= 24;

		//reference for wday : Monday 0h0
		wDay = ((epoch + 3) % 7);

		unsigned long total_days = 0;
		year=0;
		while((unsigned)(total_days += (LEAP_YEAR(year) ? 366 : 365)) <= epoch)
		{
			year++;
		}
		total_days -= LEAP_YEAR(year) ? 366 : 365;
		epoch -= total_days;

		//epoch contains here the days from january 1st, and we got the current year (minus 1970)
	}

	unsigned long day,month;
	{
		bool leapyear = LEAP_YEAR(year);

		unsigned int monthLength = 0;

		bool bool_continue = true;
		for(month = 0; (month < 12)&& bool_continue; month++)
		{
			if( (month==1) && leapyear ) {
				monthLength=daysPerMonth[month]+1;
			} else {
				monthLength=daysPerMonth[month];
			}

			if( epoch >= monthLength) {
				epoch -= monthLength;
			}
			else
			{
				bool_continue=false;
			}
		}
		day = epoch + 1;
		year += 1970;
	} //now seconds,minutes,weekday,day,month,year are fine
	//hours are still missing


	unsigned long UTC2France_conversion = getSecondsToAdjustDstEurope(year,month,day);
	unsigned long hours = ( (epoch_in+UTC2France_conversion)  % 86400L) / 3600;

	/*
	Serial.print("Date: ");
	Serial.print(day);
	Serial.print("/");
	Serial.print(month);
	Serial.print("/");
	Serial.print(year);
	Serial.print(" ");
	Serial.print(hours);
	Serial.print(":");
	Serial.print(minutes);
	Serial.print("  wDay: ");
	Serial.println(wDay);
	 */

	time_received=(wDay*SECS_PER_DAY)+hours*SECS_PER_HOUR+minutes*SECS_PER_MIN+seconds;


}

e_state_time_request myWifiForTime::getState()
{
	return state;
}
void myWifiForTime::requestTime()
{
	time_requested=true;
}

void myWifiForTime::force_ad_hoc()
{
	state=e_state_time_request_WIFI_ad_hoc_start;
}

unsigned long myWifiForTime::getTimeReceived()
{
	//basic protection
	if(state==e_state_time_request_time_available)
	{
		state=e_state_time_request_idle;
		return time_received;
	}
	return 0;
}

void myWifiForTime::setCredentials(char * ptr_ssid, char * ptr_password){
	ssid = ptr_ssid;
	password = ptr_password;
}
void myWifiForTime::setCredentialsDefault(){
	strcpy(ssid,"Not configured");
	strcpy(password,"Not configured");
}

char * myWifiForTime::get_ssid(){
	return ssid;
}
char * myWifiForTime::get_password(){
	return password;
}

void myWifiForTime::end_configuration(){
	state=e_state_time_request_WIFI_ad_hoc_finish;
}

bool myWifiForTime::update_EEPROM(){
	if(new_EEPROM_config){
		new_EEPROM_config=false;
		return true;
	} else {
		return false;
	}
}

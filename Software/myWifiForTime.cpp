/*
 * myWifiForTime.cpp
 *
 *  Created on: 8 juil. 2017
 *      Author: Jerome
 */

#include "myWifiForTime.h"

myWifiForTime::myWifiForTime() {
	// TODO Auto-generated constructor stub
	Udp=NULL;
	timeServer=NULL;

	state = e_state_time_request_idle;
	time_requested = false;

	// set all bytes in the buffer to 0
	memset(packetBuffer, 0, CONST_NTP_PACKET_SIZE);
}

void myWifiForTime::init(){
	static WiFiUDP myUDP;
	static IPAddress mytimeServer(129, 6, 15, 28); // time.nist.gov NTP server
	Udp=&myUDP;
	timeServer=&mytimeServer;


	//default : no wifi
	WiFi.forceSleepBegin();
}

void myWifiForTime::update(){
	switch(state){
	case e_state_time_request_idle:

		if(time_requested){
			time_requested = false;

			// Connect to WiFi
			WiFi.forceSleepWake();
			WiFi.begin(ssid, password);
			state=e_state_time_request_wifi_connecting;
		}
		break;

	case e_state_time_request_wifi_connecting:
		if (WiFi.status() == WL_CONNECTED){
			Udp->begin(CONST_PORT_FOR_UDP_PACKET);
			sendNTPpacket(timeServer); // send an NTP packet to a time server

			state = e_state_time_request_waiting_UDP_response;
		}
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

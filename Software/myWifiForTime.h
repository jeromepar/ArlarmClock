/*
 * myWifiForTime.h
 *
 *  Created on: 8 juil. 2017
 *      Author: Jerome
 */

#ifndef MYWIFIFORTIME_H_
#define MYWIFIFORTIME_H_

#include "ESP8266WiFi.h"
#include <ESP8266WebServer.h>
#include <WiFiUdp.h>
#include <DNSServer.h>
#include "Alarmmanagement.h"
#include "timer.h"

#include "config.h" //for VERBOSE


// DNS server
#define DNS_PORT 53

#define CONST_SSID_SIZE 33 /* up to 32 char */
#define CONST_PWD_SIZE 65 /* up to 64 char */

#define SECS_PER_MIN  60
#define SECS_PER_HOUR 3600
#define SECS_PER_DAY  86400
#define SECS_PER_WEEK 604800

#define CONST_PORT_FOR_UDP_PACKET	2390  // local port to listen for UDP packets
#define CONST_NTP_PACKET_SIZE 48 // NTP time stamp is in the first 48 bytes of the message
#define CONST_NTP_TIMEZONE 2	// France is in GMT +2

#define CONST_GMT_TO_DST 7200
#define CONST_GMT_TO_NON_DST 3600

#define LEAP_YEAR(Y) (((1970+Y)>0) && !((1970+Y)%4) && (((1970+Y)%100) || !((1970+Y)%400)))

typedef enum {
	e_state_time_request_idle,
	e_state_time_request_wifi_connecting,
	e_state_time_request_waiting_UDP_response,
	e_state_time_request_time_available,
	e_state_time_request_WIFI_ad_hoc_start,
	e_state_time_request_WIFI_ad_hoc_running,
	e_state_time_request_WIFI_ad_hoc_finish
} e_state_time_request;

class myWifiForTime {
public:
	myWifiForTime();
	void init(Alarm_management *AM);
	void update();
	e_state_time_request getState();
	void requestTime();
	unsigned long getTimeReceived();
	void force_ad_hoc();
	Alarm_management *alarms;
	void setCredentials(char * ptr_ssid, char * ptr_password);
	void setCredentialsDefault();
	char * get_ssid();
	char * get_password();
	void end_configuration();
	bool update_EEPROM();

private:

	WiFiUDP *Udp;
	IPAddress *timeServer;
	byte packetBuffer[CONST_NTP_PACKET_SIZE]; //buffer to hold incoming and outgoing packets
	const unsigned long daysPerMonth[12]= {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
	e_state_time_request state;
	bool time_requested;
	unsigned long time_received;
	bool new_EEPROM_config;


	void sendNTPpacket(IPAddress *address);
	int getSecondsToAdjustDstEurope(int year, int month, int day);
	unsigned long get_epoch_time_from_buffer();
	void calculate_time(unsigned long epoch);

	char* ssid; 	//actual location : static struc to/from eeprom - placed at init
	char* password;	//actual location : static struc to/from eeprom - placed at init
};

#endif /* MYWIFIFORTIME_H_ */

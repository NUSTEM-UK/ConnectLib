/**
 * Connect.h - Library for Connect devices
 * Created by Jonathan Sanderson, April 2020
 * Northumbria University 
 * See https://nustem.uk/connect
 * TODO: License
*/

#ifndef Connect_h
#define Connect_h

#include "Kniwwelino.h" // Includes Arduino.h for us
#include <ServoEasing.h>
#include "arduino_secrets.h" // #define MQTT_PASS, .gitignored

#define WIFI_ON 1

// Redefine MQTT stuff previously set in Kniwwelino.h
// TODO: check Kniwwelino.MQTTsetup()
#define DEF_UPDATESERVER		"connect.nustem.uk"
#define DEF_MQTTSERVER		 	"connect.nustem.uk"
#define DEF_MQTTPORT			1883
#define DEF_MQTTUSER			"connect"
#define DEF_MQTTPW		        "MQTT_PASS"
#define DEF_MQTTPUBLICDELAY		300
#define DEF_MQTTBASETOPIC		"kniwwelino/"
#define DEF_FWUPDATEURL      	"/updateFW"
#define DEF_CONFUPDATEURL       "/updateConf"

#define NTP_SERVER			  	"uk.pool.ntp.org"
#define NTP_PORT			  	8888
// TODO: work out what this is and if it's used:
// (NTP_TIMEZONE doesn't appear to be used by Kniwwelino or Timezone libraries.
// Check against actual time received. May need to do something clever with BST,
// But that should be baked into the Timezone library if we call it correctly.)
#define NTP_TIMEZONE			1 
#define NTP_PACKET_SIZE			48 // NTP time is in the first 48 bytes of message

#define HAPPY "B0000001010000001000101110"
#define SAD "B0000001010000000111010001"
#define HEART "B0101011111111110111000100"
#define SKULL "B0111010101111110111001110" 
#define DUCK "B0110011100011110111000000"

class Connect{
public:
    Connect(int pin); //root object, needs variable stubs
    void begin();
    // void func1();
    // void func2();
private:
    int _pin;
};

void init(void);



#endif
/**
 * Connect.h - Library for Connect devices
 * Created by Jonathan Sanderson, July 2021
 * Northumbria University
 * See https://nustem.uk/connect
 * TODO: License
*/

#ifndef Connect_h
#define Connect_h

#include <Kniwwelino.h>         // Includes Arduino.h for us
#include <ServoEasing.h>
#include "arduino_secrets.h"    // #define MQTT_PASS, .gitignored

#define VERSION "0.01"
#define WIFI_ON 1

// Redefine MQTT stuff previously set in Kniwwelino.h
// TODO: check Kniwwelino.MQTTsetup()
// FIXME: Horrid mess here, see "2021-06-23 Testing against NUSTEM again.md" for notes
// TL;DR: MQTTsetup() has been removed, so the only way to set this stuff is to
//        hard-code it in KniwwelinoLib. Ugh.
// #define DEF_UPDATESERVER		"connect.nustem.uk"
// #define DEF_MQTTSERVER		 	"connect.nustem.uk"
// #define DEF_MQTTPORT			1883
// #define DEF_MQTTUSER			"connect"
// #define DEF_MQTTPW		        "MQTT_PASS"
// #define DEF_MQTTPUBLICDELAY		300
// #define DEF_MQTTBASETOPIC		"kniwwelino/"
// #define DEF_FWUPDATEURL      	"/updateFW"
// #define DEF_CONFUPDATEURL       "/updateConf"

#define NTP_SERVER			  	"uk.pool.ntp.org"
#define NTP_PORT			  	8888
// TODO: work out what this is and if it's used:
// (NTP_TIMEZONE doesn't appear to be used by Kniwwelino or Timezone libraries.
// Check against actual time received. May need to do something clever with BST,
// But that should be baked into the Timezone library if we call it correctly.)
#define NTP_TIMEZONE			1
#define NTP_PACKET_SIZE			48 // NTP time is in the first 48 bytes of message

#define NUM_MOODS               5
// #define HAPPY "B0000001010000001000101110"
// #define SAD "B0000001010000000111010001"
// #define HEART "B0101011111111110111000100"
// #define SKULL "B0111010101111110111001110"
// #define DUCK "B0110011100011110111000000"

class ConnectLib{
public:
    ConnectLib(); //root object, needs variable stubs
    void begin();
    void handleButtons();
    void changeMood();
    void loop();                        //TODO: Implement

    // Getters and setters
    String getMood();                   //TODO: Implement
    String setMood(String);             //TODO: Implement
    String getNetworkMood();            //TODO: Implement
    String setNetworkMood(String);      //TODO: Implement
    String getDisplayedMood();          //TODO: Implement
    String setDisplayedMood(String);    //TODO: Implement

    // Utility

    // Moods
    void doHappy();                     //TODO: Implement
    void doSad();                       //TODO: Implement
    void doHeart();                     //TODO: Implement
    void doSkull();                     //TODO: Implement
    void doDuck();                      //TODO: Implement

    // Queue manipulations
    void pushAction(...);               //FIXME: Variables?
    void popAction(...);                //FIXME: Variables? Will need typedef

private:
    // int _pin;
    static void messageReceived(String &s, String &t);
};

extern ConnectLib Connect;

// void init(void);



#endif

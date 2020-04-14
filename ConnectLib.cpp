// Save this to Libraries directory

#include "Arduino.h"
#include "Kniwwlino.h"
#include "Connect.h"
#include "arduino_secrets.h"

#define WIFI_ON 1

// Below set to defaults, will need overriding for conNecT
// (these are defined in Kniwwelino.h)
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
#define NTP_TIMEZONE			1
#define NTP_PACKET_SIZE			48 // NTP time is in the first 48 bytes of message

int my_mood;
String my_icon;
String network_mood;
String displayed_mood;
String received_string;

// Constructor
Connect::Connect(int pin)
{
    // Setup stuff here
}

oid Connect::method()
{
    // Class method code
}

void connectInit() {
    Kniwwelino.RGBsetBrightness((int)200);
    Kniwwelino.RGBclear();
    Kniwwelino.MATRIXdrawIcon(ICON_SMILE)
    #if WIFI_ON
    Kniwwelino.MQTTpublish("hello_my_name_is", String(Kniwwelino.getMAC()));
    #endif
    int my_mood = 0;

}

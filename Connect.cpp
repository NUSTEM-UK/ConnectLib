// Save this to Libraries directory

#include "Connect.h" // Includes Kniwwelino.h and, via that, Arduino.h

#define WIFI_ON 1


int my_mood;
String my_icon;
String network_mood;
String displayed_mood;
String received_string;

// Constructor
Connect::Connect(int pin)
{
    // Setup stuff here
    int my_mood;
    String my_icon;
    String network_mood;
    String displayed_mood;
    String received_string;
}

// void Connect::method()
// {
//     // Class method code
// }

// Call in setup()
void Connect::begin() {
    Kniwwelino.begin("connect_object", WIFI_ON, true, false); // Wifi=true, Fastboot=true, MQTT Logging=false
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("START " __FILE__ "\r\nVersion " VERSION " from " __DATE__));

    #if WIFI_ON
        Kniwwelino.MQTTsetGroup(String("NUSTEM"));
        Kniwwelino.MQTTonMessage(messageReceived);
        Kniwwelino.MQTTsubscribe("MESSAGE");
        Kniwwelino.MQTTsubscribe("MOOD");
    #endif

    Kniwwelino.RGBsetBrightness((int)200);
    Kniwwelino.RGBclear();
    Kniwwelino.MATRIXdrawIcon(ICON_SMILE);
    #if WIFI_ON
    Kniwwelino.MQTTpublish("hello_my_name_is", String(Kniwwelino.getMAC()));
    #endif
    my_mood = 0;

}

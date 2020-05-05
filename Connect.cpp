// Save this to Libraries directory

#include "Connect.h" // Includes Kniwwelino.h and, via that, Arduino.h

#define WIFI_ON 1


int my_mood;
String my_icon;
String network_mood;
String displayed_mood;
String received_string;

struct mood {
    int mood_index;
    String mood_icon;
    String mood_text;
}

// Constructor
ConnectLib::ConnectLib()
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
void ConnectLib::begin() {
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

void ConnectLib::handleButtons() {
    if (Kniwwelino.BUTTONAclicked()) {
        changeMood();
    }
    if (Kniwwelino.BUTTONABclicked()) {
        Kniwwelino.MATRIXdrawIcon(ICON_ARROW_UP);
        Kniwwelino.sleep((unsigned long)500);
        #if WIFI_ON
        Kniwwelino.MQTTpublish("MOOD", String(my_icon));
        #else
        network_mood = String(my_icon);
        #endif
    }
}

void ConnectLib::changeMood() {
    my_mood += 1;
    // loop around moods
    if (my_mood > 4) {
        my_mood = 0;
    }
    switch (my_mood) {
    case 0:
        my_icon = String(HAPPY);
        break;
    case 1:
        my_icon = String(SAD);
        break;
    case 2:
        my_icon = String(HEART);
        break;
    case 3:
        my_icon = String(SKULL);
        break;
    case 4:
        my_icon = String(DUCK);
        break;
    default:
        break;
    }

    Kniwwelino.MATRIXdrawIcon(my_icon);
    Kniwwelino.sleep((unsigned long) 1000);
    Kniwwelino.MATRIXdrawIcon(network_mood);
}

String ConnectLib::getMood() {
//TODO: Implement
return my_mood;
}

String ConnectLib::setMood(String newMood) {
//TODO: Implement
return ;
}

String ConnectLib::getNetworkMood() {
//TODO: Implement
}

String ConnectLib::setNetworkMood(String newMood) {
//TODO: Implement
}

String ConnectLib::getDisplayedMood() {
//TODO: Implement
}

String ConnectLib::setDisplayedMood(String newMood) {
//TODO: Implement
}


// TODO: replace this with Kniwwelino calls rather than rolling my own.
static void messageReceived(String &topic, String &payload) {
    if (topic == "MESSAGE") {
        received_string = payload;
    } else if (topic == "MOOD") {
        network_mood = payload;
    }
    
}

// pre-instantiate object
// cf. KniwwelinoLib

ConnectLib Connect = ConnectLib();
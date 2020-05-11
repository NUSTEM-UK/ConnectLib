// Save this to Libraries directory

#include "Connect.h" // Includes Kniwwelino.h and, via that, Arduino.h

#define WIFI_ON 1


int my_mood;
String my_icon;
String network_mood;
String displayed_mood;
String received_string;

struct Mood {
    int index;
    String text;
    String icon;
    String function;
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
        Kniwwelino.MQTTonMessage(messageReceived); // throw the callback
        Kniwwelino.MQTTsubscribe("MESSAGE");
        Kniwwelino.MQTTsubscribe("MOOD");
    #endif

    // configure moods
    Mood moods[5] = {
        {0, "HAPPY", "B0000001010000001000101110", "beHappy"},
        {1, "SAD", "B0000001010000000111010001", "beSad"}
        {2, "HEART", "B0101011111111110111000100", "beHeart"}
        {3, "SKULL", "B0111010101111110111001110" , "beSkull"}
        {4, "DUCK", "B0110011100011110111000000", "beDuck"}
    }

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

int ConnectLib::setMoodHandler(String thisMood, String thisMethod) {
    // TODO: method prototype
    int targetMood = getIndexForMood(thisMood);
    if (targetMood != -1) {
        moods[targetMood].text = thisMood;
        moods[targetMood].function = thisMethod;
    }
    return targetMood;
}

String ConnectLib::getMoodHandler(String thisMood) {
    // TODO: method prototype
    int targetMood = getIndexForMood(thisMood);
    if (targetMood != -1) {
        return moods[targetMood].function;
    }
}

int getIndexForMood(String thisMood) {
    for (int i = 0; i < NUM_MOODS; i++) {
        if (moods[i].text == thisMood) {
            return i;
        }
    }
    // TODO: handle adding moods to the end of the array.
    // Ugh, need to have a MAX_MOODS variable here.
    return ;
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
// ...though this is Kniwwelino callback, if we register messageReceived accordingly.
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
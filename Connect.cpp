#include "Connect.h"
#include <kniwwelino.h>

// We need to keep track of the number of moods manually, because C
// ...and this has to be const for the array declaration to work
// ...and we need the array declaration here because we want moods[]
// to be global
// const int numberOfMoods = NUMBER_OF_MOODS; // Have to keep track of this manually, because C.
// Mood moods[numberOfMoods];   // Initialiser has to be inside setup(), or bad things happen.

// Constructor
ConnectLib::ConnectLib() {
    // Initialise the mood array. Have to do this in the constructor for the library
    // or we get horrible linking errors.
    // To add moods, append them to this array, and adjust NUMBER_OF_MOODS
    // accordingly in ConnectLib.h
    static Mood moods[NUMBER_OF_MOODS] = {
        {0, "HAPPY", "B0000001010000001000101110", &ConnectLib::doHappy},
        {1, "SAD", "B0000001010000000111010001", &ConnectLib::doSad},
        {2, "HEART", "B0101011111111110111000100", &ConnectLib::doHeart},
        {3, "SKULL", "B0111010101111110111001110", &ConnectLib::doSkull},
        {4, "DUCK", "B0110011100011110111000000", &ConnectLib::doDuck}
    };
    Mood myMood;        // Should really be intrinsicMood or internalMood
    Mood extrinsicMood; // The current network mood. Should be externalMood?
    Mood performedMood; // The current displayed mood

    String receivedString;

}

// Initialiser. Call in setup()
void ConnectLib::begin() {
    Kniwwelino.begin("connect_object", WIFI_ON, true, false);
    Serial.begin(115200);
    Serial.println();
    Serial.println(F("-------"));
    Serial.println(F("START " __FILE__ "\r\nVersion " VERSION " from " __DATE__));
    Serial.println(F("-------"));

    Serial.println(F("MQTT server overridden in custom KniwwelinoLib"));
    Serial.println(DEF_MQTTSERVER);
    Serial.println(DEF_MQTTPORT);
    Serial.println(DEF_MQTTUSER);
    Serial.println(F("---------"));

    #if WIFI_ON
    Kniwwelino.MQTTsetGroup(String("NUSTEM"));
    Kniwwelino.MQTTonMessage(messageReceived);
    Kniwwelino.MQTTsubscribe("MESSAGE");
    Kniwwelino.MQTTsubscribe("MOOD");
    #endif

    // TODO: replace with an internal call
    Kniwwelino.RGBsetBrightness((int)200);
    Kniwwelino.RGBclear();
    Kniwwelino.MATRIXdrawIcon(ICON_SMILE);
    #if WIFI_ON
    Kniwwelino.MQTTpublish("hello_my_name_is", String(Kniwwelino.getMAC()));
    #endif


    // Start happy, because we're optimistic about the world
    myMood = moods[0];
    extrinsicMood = moods[0];
    performedMood = moods[0];

    // Cross-check that we have moods correctly. TODO: Debug code, remove
    for (size_t i = 0; i < NUMBER_OF_MOODS; i++) {
        Serial.print(moods[i].text);
        Serial.print(F(" : "));
    }
    Serial.println();
}

void ConnectLib::loop() {
    handleButtons();
    checkMood();
    Kniwwelino.loop();

}

void ConnectLib::handleButtons() {
    if (Kniwwelino.BUTTONAclicked()) {
        Serial.println(F(">>>BUTTON press: A"));
        changeMood();
    }
    if (Kniwwelino.BUTTONBclicked()) {
        Serial.println(F(">>>BUTTON press: B"));
        Kniwwelino.MATRIXdrawIcon(ICON_ARROW_UP);
        Kniwwelino.sleep((unsigned long)500);
        #if WIFI_ON
        Kniwwelino.MQTTpublish("MOOD", myMood.text); // May need to reorder this
        #else
        extrinsicMood = myMood;
        // network_mood = String(my_icon);
        #endif
    }
}

void ConnectLib::changeMood() {
    // Increment mood. Note that we can't just increment
    // myMood.index, that would break the connection to the
    // other data in the struct
    int tempMoodIndex = myMood.index;
    tempMoodIndex++;
    // Loop around moods (self-adjusting for number of moods.)
    // Note the obvious off-by-one error here, which took me
    // much longer to spot than now seems reasonable.
    if (tempMoodIndex > (NUMBER_OF_MOODS-1)) {
        tempMoodIndex = 0;
    }
    Serial.println(F(">>>Changing mood"));
    Serial.print(F(">>>New mood is: "));
    Serial.println(moods[tempMoodIndex].text);
    // Display our new internal mood
    myMood = moods[tempMoodIndex];
    Kniwwelino.MATRIXdrawIcon(myMood.icon);
    Kniwwelino.sleep((unsigned long) 1000);
    Serial.println(F(">>>Returning to extrinsicMood"));
    // Now return to the network mood
    Kniwwelino.MATRIXdrawIcon(extrinsicMood.icon);
}

void ConnectLib::checkMood() {
    // Serial.println("Mood check");
    if (extrinsicMood.index != performedMood.index) {
        // We have a new mood to represent
        performedMood = extrinsicMood;
        myMood = extrinsicMood;
        // Display the associated icon
        Kniwwelino.MATRIXdrawIcon(performedMood.icon);
        // Now call the associated action function.
        (performedMood.callback)();
        // Display the associated icon
        // Do this after the callback too, in case we want to do
        // something funky along the way.
        Kniwwelino.MATRIXdrawIcon(performedMood.icon);
    }
}

void ConnectLib::messageReceived(String &topic, String &message) {
    Serial.println(F("---> MESSAGE RECEIVED"));

  if (topic=="MESSAGE") {
    receivedString = message;
  } else if (topic=="MOOD") {
      //   Serial.println(F("Got a mood"));
      int tempIndex = getMoodIndexFromString(message);
      if (tempIndex != -1) {
          extrinsicMood = moods[tempIndex];
      }
      // Serial.print(F("Mood is: "));
      // Serial.println(tempIndex);

      // network_mood = message;
  }
}


int ConnectLib::getMoodIndexFromString(String moodString) {
    int foundMoodIndex = -1; // Initialise
    int i = 0;
    // Serial.print(F("Checking for index for Mood: "));
    // Serial.println(moodString);

    while (i < NUMBER_OF_MOODS) {
        // This is ugly because we always loop through the entire array,
        // but the obvious alternatives seem worse
        // TODO: find an obvious alternative that isn't worse.
        // Serial.print(F("Stepping through: "));
        // Serial.print(i);
        // Serial.print(moods[i].text);

        if (moodString == moods[i].text) {
            // Serial.print(F("  Found a match at "));
            // Serial.println(i);
            foundMoodIndex = i;
        } else {
            // Serial.print(F("  Did not find a match at "));
            // Serial.println(i);
        }
        i++;
    }
    // Will return match or -1 if no match
    if (foundMoodIndex != -1) {
        // Serial.print(F("Got a matching mood: "));
        // Serial.println(moods[foundMoodIndex].text);
    }

    return foundMoodIndex;
}

void ConnectLib::doHappy() {
    // TODO: default action here
    Serial.println(F("New mood received: HAPPY"));
}

void ConnectLib::doSad() {
    // TODO: default action here
    Serial.println(F("New mood received: SAD"));
}

void ConnectLib::doHeart() {
    // TODO: default action here
    Serial.println(F("New mood received: HEART"));
}

void ConnectLib::doSkull() {
    // TODO: default action here
    Serial.println(F("New mood received: SKULL"));
}

void ConnectLib::doDuck() {
    // TODO: default action here
    Serial.println(F("New mood received: DUCK"));
}

// pre-instantiate object
// cf. KniwwelinoLib
ConnectLib Connect = ConnectLib();

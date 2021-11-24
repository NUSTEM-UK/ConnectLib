/* Connect.ino - bundle of helper functions for Connected devices,
built on top of Kniwwelino. This should probably be a separate library,
but that got ... hard.

Jonathan Sanderson, Northumbria University, July 2021
TODO: License
*/

// #include <Kniwwelino.h>
// #include <Arduino.h>
// #include <ServoEasing.h>
#include "Connect.h"

// Initialise the mood array. Doesn't work correctly if I move this to
// above setup() (first string is truncated).
// To add moods, append them to this array, and adjust NUMBER_OF_MOODS
// accordingly.
Mood moods[NUMBER_OF_MOODS];

// Start happy, because we're optimistic about the world
Mood myMood = moods[0];
Mood extrinsicMood = moods[0];
Mood performedMood = moods[0];

String received_string = "";

// TODO: check if this is part of the Kniwwelino base code
static void messageReceived(String &topic, String &payload) {
    Serial.println(F("---> MESSAGE RECEIVED"));

  if (topic=="MESSAGE") {
    received_string = payload;
  } else if (topic=="MOOD") {
      //   Serial.println(F("Got a mood"));
      int tempIndex = getMoodIndexFromString(payload);
      if (tempIndex != -1) {
          extrinsicMood = moods[tempIndex];
      }
      // Serial.print(F("Mood is: "));
      // Serial.println(tempIndex);

      // network_mood = payload;
  }
}


int getMoodIndexFromString (String moodString) {
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


void change_mood() {
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

void handleButtons() {
    // Serial.println("Button check");
    if (Kniwwelino.BUTTONAclicked()) {
        Serial.println(F(">>>BUTTON press: A"));
        change_mood();
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

void checkMood() {
    // Serial.println("Mood check");
    if (extrinsicMood.index != performedMood.index) {
        // We have a new mood to represent
        performedMood = extrinsicMood;
        myMood = extrinsicMood;
        // Display the associated icon
        Kniwwelino.MATRIXdrawIcon(performedMood.icon);
        // Now call the associated action function.
        performedMood.callback();
        // Display the associated icon
        // Do this after the callback too, in case we want to do
        // something funky along the way.
        Kniwwelino.MATRIXdrawIcon(performedMood.icon);
    }
}

void connectSetup() {
    Kniwwelino.begin("Connected_Device", WIFI_ON, true, false); // Wifi=true, Fastboot=true, MQTT Logging=false
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

    Kniwwelino.RGBsetBrightness((int)200);
    Kniwwelino.RGBclear();

    moods[0] = {0, "HAPPY", "B0000001010000001000101110", &doHappy};
    moods[1] = {1, "HEART", "B0101011111111110111000100", &doHeart};
    moods[2] = {2, "SAD", "B0000001010000000111010001", &doSad};
    moods[3] = {3, "SKULL", "B0111010101111110111001110", &doSkull};
    moods[4] = {4, "SILLY", "B0101000000111110001100011", &doSilly};
    moods[5] = {5, "DUCK", "B0110011100011110111000000", &doDuck};

    // Cross-check that we have moods correctly.
    for (size_t i = 0; i < NUMBER_OF_MOODS; i++) {
        Serial.print(moods[i].text);
        Serial.print(F(" : "));
    }
    Serial.println();

}

void connectLoop() {
    updateConnectServos();
    handleButtons();
    // TODO: Think about whether this gets called in the loop,
    //       or only from messageReceived(). The latter would
    //       seem more appropriate and performative?
    checkMood();
    updateConnectServos();
    Kniwwelino.loop();
    // It's really clunky to call update ~twice~ three times in the loop, but otherwise
    // the servos don't get enough time. Yikes.
    // twice is almost good enough, maybe
    // TODO: troubleshoot this.
    updateConnectServos();
}

/**
 * ACTION STUBS
 * These are all defined weak so they can be overriden
 * in the user sketch. Ah, C - there's always a way.
 * (virtual methods in a class might be preferable,
 * but this works for now.)
*/

__attribute__ ((weak))
void doHappy() {
    Serial.println(F("New mood received: HAPPY"));
    // TODO: Default waving behaviour here?
    // servos_engage();
    // servo1Speed = 100;)
    // for (int i = 0; i < 3; i++) {
    //     Servo1.startEaseTo(180, servo1Speed, true);
    //     Kniwwelino.RGBsetColorEffect(String("00FF00"), RGB_BLINK, -1);
    //     while (Servo1.isMovingAndCallYield()) {
    //         // Nothing here, intentionally
    //     }
    //     Servo1.startEaseTo(0, servo1Speed, true);
    //     Kniwwelino.RGBsetColorEffect(String("FF0000"), RGB_GLOW, -1);
    //     while (Servo1.isMovingAndCallYield()) {
    //         // Nothing here, intentionally
    //     }
    // }
    // Kniwwelino.RGBclear(); // Turn the LED off.
    // servos_disengage();
}

__attribute__ ((weak))
void doSad() {
    Serial.println(F("New mood received: SAD"));
}

__attribute__ ((weak))
void doHeart() {
    Serial.println(F("New mood received: HEART"));
}

__attribute__ ((weak))
void doSkull() {
    Serial.println(F("New mood received: SKULL"));
}

__attribute__ ((weak))
void doSilly() {
    Serial.println(F("New mood received: SILLY"));
}

__attribute__ ((weak))
void doDuck() {
    Serial.println(F("New mood received: DUCK"));
}


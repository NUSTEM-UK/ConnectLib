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

bool isInverted = false; // Is the Kniwwelino mounted upside-down?

String received_string = "";
SoftwareSerial myPort(RX_PIN, TX_PIN, false, 256);
bool isSerialZombie = false;
String received;
char incomingChar = 0;
int receivedMoodAnimationRate = 75;


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
        // Play the received mood animation
        receivedMoodWiggleAnimation();
        // Act on the received mood
        checkMood();
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
    // 2022-01-17: edited to skip over DUCK mood. Still in there,
    //             but no longer accessible.
    if (tempMoodIndex > (NUMBER_OF_MOODS-2)) {
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


void publish_mood() {
    // Publish our mood to the network
    // Arrow direction depends on whether we're inverted or not
    if (isInverted) {
        Kniwwelino.MATRIXdrawIcon(ICON_ARROW_DOWN);
    } else {
        Kniwwelino.MATRIXdrawIcon(ICON_ARROW_UP);
    }
    Kniwwelino.sleep((unsigned long)500);
    #if WIFI_ON
    Kniwwelino.MQTTpublish("MOOD", myMood.text); // May need to reorder this
    #else
    extrinsicMood = myMood;
    // network_mood = String(my_icon);
    #endif
}

void handleButtons() {
    if (Kniwwelino.BUTTONAclicked()) {
        Serial.println(F(">>>BUTTON press: A"));
        // By default, change the mood. If we're inverted, publish instead.
        isInverted ? publish_mood() : change_mood();
    }
    if (Kniwwelino.BUTTONBclicked()) {
        Serial.println(F(">>>BUTTON press: B"));
        // By default, publish the mood. If we're inverted, change it instead.
        isInverted ? change_mood() : publish_mood();
    }
}

void checkMood() {
    // Serial.println("Mood check");

    // ----- CUT HERE -----
    // For debugging/development, we ran as this:
    // Check if new network mood is different from performed mood
    // if (extrinsicMood.index != performedMood.index) {
    //     // We have a new mood to represent
    //     performedMood = extrinsicMood;
    //     myMood = extrinsicMood;
    //     // Display the associated icon
    //     Kniwwelino.MATRIXdrawIcon(performedMood.icon);
    //     // Now call the associated action function.
    //     performedMood.callback();
    //     // Display the associated icon
    //     // Do this after the callback too, in case we want to do
    //     // something funky along the way.
    //     Kniwwelino.MATRIXdrawIcon(performedMood.icon);
    // }
    // ----- CUT HERE -----

    // ...but for deployment, we now thing we want to respond
    // to every mood received. This could get mad, and probably
    // needs a rate limit.
    // FIXED: rate limit. Now checked when messageReceived(), not in loop.
    performedMood = extrinsicMood;
    myMood = extrinsicMood;
    performedMood.callback();
    // TODO: Animate the mood change
    Kniwwelino.MATRIXdrawIcon(performedMood.icon);

}

void checkSerialConnection() {
    // Has anything arrived over the software serial port?
    // Serial.println(F("Checking serial connection"));
    if (myPort.available() > 0) {
        // Read incoming and append to string
        Serial.println(F("Read character"));
        incomingChar = myPort.read();
        received += incomingChar;

        // Check if we've received a newline terminator
        if (incomingChar == '\n'){
            // Trim the closing newline
            received.trim();
            // Now parse the received string, looking for ACK
            if (received == F("ACK")) {
                Serial.println(received);
                Serial.println(F(" >>> CEDING CONTROL"));
                delay(100); // Can be to fast for the Pico to handle?
                // Respond in kind
                myPort.write("ACK ACK ACK");
                // Turn off Connect mood messaging stuff
                // TODO: Check if this can be F'd. Tripped a compiler error when I tried.
                Kniwwelino.MQTTunsubscribe("MOOD");
                // Display hint we're in zombie mode
                // 1 1 1 1 1
                // 0 0 0 1 0
                // 0 0 1 0 0
                // 0 1 0 0 0
                // 1 1 1 1 1
                Kniwwelino.MATRIXdrawIcon(F("B1111100010001000100011111"));
                Kniwwelino.MATRIXsetBlinkRate(MATRIX_BLINK_1HZ);
                // Flag that we're now a serial zombie
                isSerialZombie = true;

                // SShort pause, then clear the capture string
                delay(500);
                received = "";
            } else {
                Serial.println(received);
                Serial.println(F("Missed ACK, reset and try again"));
                // Reset the capture string
                received = "";
            }
        }
    }
}

void parseSerialConnectionAndDriveDevices() {
    // Get commands
    if (myPort.available() > 0) {
        // read incoming and append to capture string
        incomingChar = myPort.read();
        received += incomingChar;

        // Check if we have a newline terminator
        if (incomingChar == '\n') {
            // Remove that closing newline
            received.trim();
            // Now send the received string to the ConnectMessenger object for handling
            ConnectMessenger.serialCommand(received);
            // Reset the capture string
            received = "";
        }
    }
}

void receivedMoodWiggleAnimation() {
    // Display a wiggle animation to indicate a mood has been received
    // TODO: This is horrid code, rewrite it so less horrid
    for (int i = 0; i < 2; i++) {
        Kniwwelino.MATRIXdrawIcon(String("B0000000000100000000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000110000000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000011000000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000001100000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000000110000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000000010000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000000110000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000001100000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000011000000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.MATRIXdrawIcon(String("B0000000000110000000000000"));
        Kniwwelino.sleep((unsigned long) receivedMoodAnimationRate);
        Kniwwelino.loop(); // do background stuff...
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
    Kniwwelino.MQTTsubscribe(F("MESSAGE"));
    Kniwwelino.MQTTsubscribe(F("MOOD"));
    #endif

    Kniwwelino.RGBsetBrightness((int)150);
    Kniwwelino.RGBclear();
    Kniwwelino.MATRIXdrawIcon(ICON_SMILE);
    #if WIFI_ON
    Kniwwelino.MQTTpublish(F("hello_my_name_is"), String(Kniwwelino.getMAC()));
    #endif
    delay(1000);

    // Check if we're inverted and assign mood icons accordingly.
    if (isInverted)
    {
        Serial.println(F(">>> INVERTED, setting icons accordingly"));
        moods[0] = {0, F("HAPPY"), F("B0111010001000000101000000"), &doHappy};
        moods[1] = {1, F("HEART"), F("B0010001110111111111101010"), &doHeart};
        moods[2] = {2, F("SAD"),   F("B1000101110000000101000000"), &doSad};
        moods[3] = {3, F("SKULL"), F("B0111001110111111010101110"), &doSkull};
        moods[4] = {4, F("SILLY"), F("B0001100011111110000001010"), &doSilly};
        moods[5] = {5, F("DUCK"),  F("B0000001110011111110001100"), &doDuck};
    } else {
        moods[0] = {0, F("HAPPY"), F("B0000001010000001000101110"), &doHappy};
        moods[1] = {1, F("HEART"), F("B0101011111111110111000100"), &doHeart};
        moods[2] = {2, F("SAD"), F("B0000001010000000111010001"), &doSad};
        moods[3] = {3, F("SKULL"), F("B0111010101111110111001110"), &doSkull};
        moods[4] = {4, F("SILLY"), F("B0101000000111110001100011"), &doSilly};
        moods[5] = {5, F("DUCK"), F("B0110011100011110111000000"), &doDuck};
    }


    // Cross-check that we have moods correctly.
    for (size_t i = 0; i < NUMBER_OF_MOODS; i++) {
        Serial.print(moods[i].text);
        Serial.print(F(" : "));
    }

    // Set up software serial connection for trainer bot
    myPort.begin(57600);
    if (!myPort) {
        Serial.println(F("Invalid SoftwareSerial config"));
        // while (1) {
        //     // Stop here
        //     delay(1000);
        // }
        // Keep going, assume we never want serial. Ulp.
    }
    Serial.println();
    Serial.println(F("SoftwareSerial started"));

    // Confirm we're not currently zombied
    isSerialZombie = false;

    Serial.println();

}

void connectLoop() {
    // Update Kniwwelino stuff
    Kniwwelino.loop();

    // Everything else depends on serial zombie state
    if (isSerialZombie) {
        // We *are* a serial zombie, so do as bidden
        parseSerialConnectionAndDriveDevices();
    } else {
        // Do normal things
        handleButtons();
        ConnectMessenger.updateServos();
        // ...and check the SoftwareSerial port in case we're being zombied
        checkSerialConnection();
        // debugSerialConnection();
    }
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

void setInverted(bool inverted) {
    isInverted = inverted;
    if (isInverted)
    {
        Serial.println(F(">>> INVERTED, setting icons accordingly"));
        moods[0] = {0, F("HAPPY"), F("B0111010001000000101000000"), &doHappy};
        moods[1] = {1, F("HEART"), F("B0010001110111111111101010"), &doHeart};
        moods[2] = {2, F("SAD"),   F("B1000101110000000101000000"), &doSad};
        moods[3] = {3, F("SKULL"), F("B0111001110111111010101110"), &doSkull};
        moods[4] = {4, F("SILLY"), F("B0001100011111110000001010"), &doSilly};
        moods[5] = {5, F("DUCK"),  F("B0000001110011111110001100"), &doDuck};
    } else {
        Serial.println(F(">>> NOT INVERTED, setting icons accordingly"));
        moods[0] = {0, F("HAPPY"), F("B0000001010000001000101110"), &doHappy};
        moods[1] = {1, F("HEART"), F("B0101011111111110111000100"), &doHeart};
        moods[2] = {2, F("SAD"), F("B0000001010000000111010001"), &doSad};
        moods[3] = {3, F("SKULL"), F("B0111010101111110111001110"), &doSkull};
        moods[4] = {4, F("SILLY"), F("B0101000000111110001100011"), &doSilly};
        moods[5] = {5, F("DUCK"), F("B0110011100011110111000000"), &doDuck};
    }
}

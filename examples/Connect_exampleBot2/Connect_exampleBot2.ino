#include <Connect.h>
#include "ServoEasing.hpp"

// Define two servos only; the second is jittery with a Kniwwelino,
// and we're not going to manage a third. Even though a bare D1 could.
ConnectServo servoD5;
ConnectServo servoD7;

void setup() {
    // setInverted(true);
    servoD5.setHome(90);
    servoD7.setHome(90);

    connectSetup();

    servoD5.setPin(D5);
    servoD7.setPin(D7);
}

void loop() {
    connectLoop();
}

void doHappy() {
    servoD5.wave(3, 0, 180);
    servoD7.wave(3, 0, 180);
}

void doSad() {
    servoD5.queueEaseTo(0, EASE_CUBIC_OUT, 15);
    servoD7.flap(2, 0, 180);
}


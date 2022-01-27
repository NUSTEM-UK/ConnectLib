#include <Connect.h>

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

    Kniwwelino.sleep(500);
}

void loop() {
    connectLoop();
}

void doHappy() {
    Serial.println(F(">>> New mood received: HAPPY"));
    servoD5.queueEaseTo(135, EASE_CUBIC_IN_OUT, 60);
    servoD5.wave(3, 135, 45);
    // servo1.queueMoveTo(0);
    // servo1.queueWait(250);
    // servo2.queueMoveTo(0);
    // servo2.queueWait(250);
    // servo1.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    // servo1.queueEaseTo(0, EASE_CUBIC_IN_OUT, 90);
    // servoWaitForServo(servo2, servo1);
    // servo1.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
    // servo2.queueEaseTo(180, EASE_CUBIC_IN_OUT, 60);
}

void doSad() {
    Serial.println(F(">>> New mood received: SAD"));
    servoD5.queueEaseTo(0, EASE_CUBIC_OUT, 15);
    // servo2.queueEaseTo(180, EASE_CUBIC_IN_OUT, 20);
}

void doHeart() {
    Serial.println(F(">>> New mood received: HEART"));
}

void doSkull() {
    Serial.println(F("New mood received: SKULL"));
}

void doSilly() {
    Serial.println(F("New mood received: SILLY"));
}

void doDuck() {
    Serial.println(F("New mood received: DUCK"));
}

#include <Connect.h>

// Define two servos only; the second is jittery with a Kniwwelino,
// and we're not going to manage a third. Even though a bare D1 could.
ConnectServo servo1;
// ConnectServo servo2;

// Set pins for servos
static const uint8_t servo1Pin = D5;
// static const uint8_t servo2Pin = D7;

void setup() {
    // setInverted(true);
    connectSetup();

    servo1.setPin(servo1Pin);
    // servo2.setPin(servo2Pin);
    servo1.setHome(90);
    // servo2.setHome(90);
    Kniwwelino.sleep(500);
}

void loop() {
    connectLoop();
}

void doHappy() {
    Serial.println(F(">>> New mood received: HAPPY"));
    servo1.wave(3, 135, 45);
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
    servo1.queueEaseTo(0, EASE_CUBIC_IN_OUT, 15);
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

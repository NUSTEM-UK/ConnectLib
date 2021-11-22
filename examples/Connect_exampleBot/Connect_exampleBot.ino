// TODO: May not need Arduino.h and Kniwwelino.h explicitly here,
//       (they should be included with Connect.h),
//       but they help with VS Code verification.
#include <Arduino.h>
#include <Kniwwelino.h>
#include <Connect.h>


// Define two servos only; the second is jittery with a Kniwwelino,
// and we're not going to manage a third. Even though a bare D1 could.
ConnectServo servo1;
ConnectServo servo2;


// Define queues
servoQueueItem tempServoQueueItem;
servoQueueItem tempServoQueueItem2;

// Queue handler
void servoQueueLoop(){
    // Is servo1 stopped?
    if (!servo1.isMoving()) {

    }
}


void servos_home() {
    setEasingTypeForAllServos(EASE_CUBIC_IN_OUT);
    servo1.setSpeed(30);
    servo2.setSpeed(30);
    servo1.startEaseTo(90);
    servo2.startEaseTo(90);
    while (servo1.isMoving() || servo2.isMoving()) {
        yield();
    }
    servo1.write(90);
    servo2.write(90);
    delay(500);
    servos_disengage();
}

void setup() {
    connectSetup();

    // Attach servos and move to midpoint / home
    servo1.attach(D5);
    servo2.attach(D7);
    servo1.write(90);
    servo2.write(90);
    delay(500);
    servo1.setEasingType(EASE_CUBIC_IN_OUT);
    servo2.setEasingType(EASE_CUBIC_IN_OUT);

    Kniwwelino.MATRIXdrawIcon(ICON_SMILE);
    #if WIFI_ON
    Kniwwelino.MQTTpublish("hello_my_name_is", String(Kniwwelino.getMAC()));
    #endif

    delay(1500);
    servos_disengage();

}

void loop() {
    connectLoop();
}

void doHappy() {
    Serial.println(F(">>> New mood received: HAPPY"));
    servos_engage();
    servo1.setSpeed(120);
    servo2.setSpeed(120);
    setEasingTypeForAllServos(EASE_SINE_IN_OUT);

    for (int i = 0; i < 3; i++) {
        servo1.startEaseTo(180);
        servo2.startEaseTo(180);

        while (servo1.isMoving() || servo2.isMoving()) {
            yield();
        }

        servo1.write(0);
        servo2.write(0);
        while (servo1.isMoving() || servo2.isMoving()) {
            yield();
        }
    }

    delay(1000);
    servos_home();
    Serial.println(F("<<< HAPPY mood complete"));
}

void doSad() {
    Serial.println(F(">>> New mood received: SAD"));
    servos_engage();
    // Move to 0 fairly quickly
    servo1.setSpeed(90);
    servo2.setSpeed(45);
    setEasingTypeForAllServos(EASE_CIRCULAR_IN);

    servo1.startEaseTo(0);
    servo2.startEaseTo(0);

    // Wait for each servo to finish moving and return it to 180 when it does.
    while (servo1.isMoving() || servo2.isMoving()) {
        if (!servo1.isMoving()) {
            servo1.setEasingType(EASE_CUBIC_IN_OUT);
            servo1.startEaseTo(180);
        }
        if (!servo2.isMoving()) {
            servo2.setEasingType(EASE_CUBIC_IN_OUT);
            servo2.startEaseTo(180);
        }
        yield();
    }

    delay(1000);
    servos_home();
    Serial.println(F("<<< SAD mood complete"));
}

void doHeart() {
    Serial.println(F(">>> New mood received: HEART"));
    servos_engage();
    // Move to 0 very slowly
    servo1.setSpeed(20);
    servo2.setSpeed(40);
    setEasingTypeForAllServos(EASE_CUBIC_IN_OUT);
    servo1.startEaseTo(0);
    servo2.startEaseTo(0);

    // Wait until we've finished moving both servos
    while (servo1.isMoving() || servo2.isMoving()) {
        yield();
    }

    // Move to 180 very slowly
    servo1.startEaseTo(180);
    servo2.startEaseTo(180);
    // Wait until we've finished moving both servos
    while (servo1.isMoving() || servo2.isMoving()) {
        yield();
    }

    // Now return to 0 a bit faster
    servo1.setSpeed(90);
    servo2.setSpeed(120);
    servo1.startEaseTo(0);
    servo2.startEaseTo(0);
    // Wait until we've finished moving both servos
    while (servo1.isMoving() || servo2.isMoving()) {
        yield();
    }
    delay(1000);

    // Return servos home.
    servos_home();
    Serial.println(F("<<< HEART mood complete"));

}

void doSkull() {
    Serial.println(F("New mood received: SKULL"));
}

void doSilly() {
    Serial.println(F("New mood received: SILLY"));
}

void doDuck() {
    Serial.println(F("New mood received: DUCK"));
    Serial.println(F("This is an injected DUCK"));
}

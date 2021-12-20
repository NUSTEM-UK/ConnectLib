// FIXME: This is now obsolete code, 2021-12-20 Remove?

#include <Connect.h>

ServoEasing servo1;
ServoEasing servo2;

void setup() {
    connectSetup();

    // Attach servos and move to midpoint / home
    servo1.attach(D5);
    servo2.attach(D7);
    servo1.write(90);
    servo2.write(90);
    delay(500);

    Kniwwelino.MATRIXdrawIcon(ICON_SMILE);
    #if WIFI_ON
    Kniwwelino.MQTTpublish(F("hello_my_name_is"), String(Kniwwelino.getMAC()));
    #endif

    delay(1500);
    servos_disengage();

}

void loop() {
    connectLoop();
}

void doHappy() {
    Serial.println(F("New mood received: HAPPY"));
    servos_engage();

    servo1Speed = 200;
    servo1.setEasingType(EASE_CIRCULAR_BOUNCING);
    for (int i = 0; i < 3; i++) {
        servo1.startEaseTo(180, servo1Speed, true);
        // Kniwwelino.RGBsetColorEffect(String("00FF00"), RGB_BLINK, -1);
        while (servo1.isMovingAndCallYield()) {
            // Nothing here, intentionally
        }
        servo1.startEaseTo(0, servo1Speed, true);
        // Kniwwelino.RGBsetColorEffect(String("FF0000"), RGB_GLOW, -1);
        while (servo1.isMovingAndCallYield()) {
            // Nothing here, intentionally
        }
    }
    // Kniwwelino.RGBclear(); // Turn the LED off.
    servos_disengage();
}

void doSad() {
    Serial.println(F("New mood received: SAD"));
    servos_engage();
    servo1Speed = 100;
    servo2Speed = 50;
    servo1.setEasingType(EASE_CUBIC_IN_OUT);
    servo2.setEasingType(EASE_CUBIC_IN_OUT);
    for (int i = 0; i < 3; i++) {
        servo1.startEaseTo(180, servo1Speed, true);
        servo2.startEaseTo(180, servo2Speed, true);
        while (servo1.isMovingAndCallYield()) {
            // Nothing here
        }
        servo1.startEaseTo(0, servo1Speed, true);
        servo2.startEaseTo(0, servo2Speed, true);
        while (servo1.isMovingAndCallYield()) {
            // Nothing here
        }
    }
    servos_disengage();
}

void doHeart() {
    Serial.println(F("New mood received: HEART"));
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

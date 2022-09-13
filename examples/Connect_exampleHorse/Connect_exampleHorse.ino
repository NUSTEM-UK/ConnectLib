#include <Connect.h>

// Servo D5: head, up: 54. down: 4
// Servo D7: neck. up: 4. down: 176

ConnectServo servoFluke;
ConnectServo servoBlowhole;

void setup() {
    // setInverted(true);
    servoBlowhole.setHome(54);
    servoFluke.setHome(76);

    connectSetup();

    servoBlowhole.setPin(D5);
    servoFluke.setPin(D7);
}

void loop() {
    connectLoop();
}

void doHappy() {
    servoFluke.wave(5, 76, 96);
    servoWaitForServo(servoBlowhole, servoFluke);
    servoBlowhole.queueMoveTo(132);
}

void doSad() {
    servoBlowhole.queueEaseTo(100, EASE_CUBIC_OUT, 10);
    servoFluke.queueEaseTo(88, EASE_CUBIC_IN_OUT, 10);
    servoFluke.queueWait(1500);
    servoWaitForServo(servoFluke, servoBlowhole);
    servoBlowhole.queueMoveTo(54);
}

void doHeart() {
    servoBlowhole.queueMoveTo(132);
    for (int count = 0; count < 3; count++) {
        servoFluke.queueMoveTo(96);
        servoFluke.queueWait(50);
        servoFluke.queueMoveTo(76);
        servoFluke.queueWait(50);
    }
    servoFluke.queueMoveTo(96);
    servoFluke.queueWait(250);
    servoWaitForServo(servoFluke, servoBlowhole);
    servoBlowhole.queueEaseTo(54, EASE_CUBIC_IN_OUT, 10);
    servoFluke.queueEaseTo(76, EASE_CUBIC_IN_OUT, 4);
}

void doSkull() {
    servoBlowhole.queueMoveTo(132);
    servoFluke.queueMoveTo(96);
    for (int count = 0; count < 3; count++) {
        servoFluke.queueMoveTo(76);
        servoFluke.queueWait(50);
        servoFluke.queueMoveTo(96);
        servoFluke.queueWait(500);
    }
    servoWaitForServo(servoBlowhole, servoFluke);
    servoBlowhole.queueMoveTo(54);
    servoFluke.queueMoveTo(76);
}

void doSilly() {
    for (int count = 0; count < 5; count++) {
        servoBlowhole.queueEaseTo(132, EASE_CUBIC_IN_OUT, 50);
        servoBlowhole.queueWait(100);
        servoBlowhole.queueEaseTo(54, EASE_CUBIC_IN_OUT, 50);
        servoBlowhole.queueWait(100);
    }
    for (int count = 0; count < 3; count++) {
        servoFluke.queueEaseTo(96, EASE_CUBIC_OUT, 5);
        servoFluke.queueWait(50);
        servoFluke.queueEaseTo(76, EASE_CUBIC_OUT, 5);
        servoFluke.queueWait(50);
    }

}


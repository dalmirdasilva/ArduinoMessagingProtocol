#include "Notifier.h"
#include <Arduino.h>

unsigned char Notifier::transmitingPin = 0;
unsigned char Notifier::waitingPin = 0;

void Notifier::setup(unsigned char transmitingPin, unsigned char waitingPin) {
    Notifier::transmitingPin = transmitingPin;
    Notifier::waitingPin = waitingPin;
}

void Notifier::notifyBeginTransmiting() {
    if (transmitingPin) {
        digitalWrite(transmitingPin, HIGH);
    }
}

void Notifier::notifyEndTransmiting() {
    if (transmitingPin) {
        digitalWrite(transmitingPin, LOW);
    }
}

void Notifier::notifyBeginWaiting() {
    if (waitingPin) {
        digitalWrite(waitingPin, HIGH);
    }
}

void Notifier::notifyEndWaiting() {
    if (waitingPin) {
        digitalWrite(waitingPin, LOW);
    }
}

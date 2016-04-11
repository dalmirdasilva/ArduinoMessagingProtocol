#include "Notifier.h"
#include <Arduino.h>

#ifndef STATUS_LED_PIN
#define STATUS_LED_PIN      13
#endif

void Notifier::notifyTransmiting() {
    digitalWrite(STATUS_LED_PIN, HIGH);
}

void Notifier::unnotifyTransmiting() {
    digitalWrite(STATUS_LED_PIN, LOW);
}

void Notifier::notifyWaiting() {
    digitalWrite(STATUS_LED_PIN, HIGH);
}

void Notifier::unnotifyWaiting() {
    digitalWrite(STATUS_LED_PIN, LOW);
}

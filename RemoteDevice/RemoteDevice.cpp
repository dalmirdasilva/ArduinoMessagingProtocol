#include "RemoteDevice.h"
#include <Notifier.h>
#include <Message.h>
#include <Arduino.h>

RemoteDevice::RemoteDevice(MessageParser* parser, Message* message, OutputStream *outputStream, InputStream *inputStream) :
        parser(parser), message(message), outputStream(outputStream), inputStream(inputStream) {
}

RemoteDevice::~RemoteDevice() {
}

bool RemoteDevice::transmitMessage(Message* message) {
    Notifier::notifyTransmiting();
    message->generateNextId();
    outputStream->write(Message::START);
    outputStream->write(message->getId());
    outputStream->write(message->getType());
    outputStream->write(message->getPayloadSize());
    outputStream->write(message->getPayload(), message->getPayloadSize());
    outputStream->write(Message::END);
    Notifier::unnotifyTransmiting();
    return true;
}

bool RemoteDevice::receiveMessage(Message* message) {
    while (inputStream->available() > 0) {
        unsigned char c = (unsigned char) inputStream->read();
        if (!parser->decode(c)) {
            return false;
        }
        if (parser->getDecodedMessage(message)) {
            return true;
        }
    }
    return false;
}

bool RemoteDevice::connect(unsigned long timeout) {
    message->reset();
    message->setType(Message::CONNECT);
    transmitMessage(message);
    return waitForMessageType(Message::CONNECT, timeout);
}

bool RemoteDevice::isConnected(unsigned long timeout) {
    message->reset();
    message->setType(Message::PING);
    transmitMessage(message);
    return waitForMessageType(Message::ACK, timeout);
}

bool RemoteDevice::waitForMessageType(Message::Type type, unsigned long timeout) {
    Notifier::notifyWaiting();
    unsigned long start = millis();
    bool received = false;
    while (!(received = (receiveMessage(message) && message->getType() == type))
            && (timeout == INFINITY_TIMEOUT || start + timeout > millis()))
        ;
    Notifier::unnotifyWaiting();
    return received;
}

bool RemoteDevice::transmitWaitingForMessageType(Message *message, Message::Type type, unsigned long timeout) {
    transmitMessage(message);
    return waitForMessageType(type, timeout);
}

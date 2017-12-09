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
    Notifier::notifyBeginTransmiting();
    message->generateNextId();
    outputStream->write(Message::Mark::START);
    outputStream->write(message->getId());
    outputStream->write(message->getType());
    outputStream->write(message->getFlags());
    outputStream->write(message->getPayloadSize());
    outputStream->write(message->getPayload(), message->getPayloadSize());
    outputStream->write(Message::Mark::END);
    Notifier::notifyEndTransmiting();
    return true;
}

bool RemoteDevice::receiveMessage(Message* message) {
    while (inputStream->available() > 0) {
        unsigned char c = (unsigned char) inputStream->read();
        if (!parser->decode(c)) {

            // Parser ran into a invalid state.
            parser->reset();
            return false;
        }
        if (parser->collectDecodedMessage(message)) {
            return true;
        }
    }
    return false;
}

bool RemoteDevice::connect(unsigned long timeout) {
    message->reset();
    message->setType(Message::Type::CONNECT);
    return transmitWaitingForMessageType(message, Message::Type::GENERIC_RESPONSE, timeout);
}

bool RemoteDevice::isConnected(unsigned long timeout) {
    message->reset();
    message->setType(Message::Type::PING);
    message->setFlags(Message::Flag::REQUIRED_ACK);
    return transmitWaitingForMessageType(message, Message::Type::ANY, timeout);
}

bool RemoteDevice::waitForMessageType(Message::Type type, unsigned long timeout) {
    Notifier::notifyBeginWaiting();
    unsigned long start = millis();
    bool received = false;
    while (!(received = (receiveMessage(message) && (message->getType() & type) > 0))
            && (timeout == INFINITY_TIMEOUT || start + timeout > millis()))
        ;
    Notifier::notifyEndWaiting();
    return received;
}

bool RemoteDevice::transmitWaitingForMessageType(Message *message, Message::Type type, unsigned long timeout) {
    transmitMessage(message);
    return waitForMessageType(type, timeout);
}

#include "Message.h"

unsigned char Message::nextId = 0;

Message::Message()
        : id(0), type(0), payloadSize(0), payload(0) {
}

Message::Message(unsigned char* payload)
        : id(0), type(0), payloadSize(0), payload(payload) {
}

Message::Message(unsigned char id, unsigned char type, unsigned char payloadSize, unsigned char* payload)
        : id(id), type(type), payloadSize(payloadSize), payload(payload) {
}

void Message::reset() {
    id = 0;
    type = 0;
    payloadSize = 0;
}

unsigned char Message::getId() {
    return id;
}

void Message::setId(unsigned char id) {
    this->id = id;
}

void Message::generateNextId() {
    this->id = ++nextId;
}

unsigned char Message::getType() {
    return type;
}

void Message::setType(unsigned char type) {
    this->type = type;
}

unsigned char Message::getPayloadSize() {
    return payloadSize;
}

void Message::setPayloadSize(unsigned char payloadsize) {
    this->payloadSize = payloadsize;
}

unsigned char* Message::getPayload() {
    return payload;
}

void Message::setPayload(unsigned char* payload) {
    this->payload = payload;
}

unsigned int Message::toRaw(unsigned char* raw) {
    unsigned int i = 0;
    unsigned char j;
    raw[i++] = START;
    raw[i++] = id;
    raw[i++] = type;
    raw[i++] = payloadSize;
    for (j = 0; j < payloadSize; j++) {
        raw[i++] = payload[j];
    }
    raw[i++] = END;
    return i;
}

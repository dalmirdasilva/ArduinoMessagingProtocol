#include "MessageParser.h"
#include <Message.h>
#include <string.h>
#include <Arduino.h>

MessageParser::MessageParser(unsigned char *buf, unsigned short len) :
        buf(buf), len(len), pos(0) {
    reset();
    maxPayloadLength = len - MESSAGE_CONTROL_LENGTH;
}

void MessageParser::reset() {
    state = INITIAL;
    pos = 0;
}

short MessageParser::decodeBytes(unsigned char* buf, unsigned short len) {
    unsigned short i;
    for (i = 0; i < len; i++) {
        if (!decode(buf[i])) {
            break;
        }
    }
    return i;
}

bool MessageParser::decode(unsigned char c) {
    bool decoded = true;
    switch (state) {
        case INITIAL:
            if (c == Message::START) {
                pos = 0;
                state = START_OF_MESSAGE_MARK_PARSED;
            } else {
                decoded = false;
            }
            break;
        case START_OF_MESSAGE_MARK_PARSED:
            state = ID_PARSED;
            break;
        case ID_PARSED:
            state = TYPE_PARSED;
            break;
        case TYPE_PARSED:
            state = FLAGS_PARSED;
            break;
        case FLAGS_PARSED:
            state = PAYLOAD_LENGTH_PARSED;
            payloadLength = c;
            if (payloadLength > maxPayloadLength) {
                decoded = false;
            }
            if (payloadLength == 0) {
                state = PAYLOAD_PARSED;
            }
            break;
        case PAYLOAD_LENGTH_PARSED:
            if (--payloadLength == 0) {
                state = PAYLOAD_PARSED;
            }
            break;
        case PAYLOAD_PARSED:
            if (c == Message::END) {
                state = END_OF_MESSAGE_MARK_PARSED;
            } else {
                decoded = false;
            }
            break;
        case END_OF_MESSAGE_MARK_PARSED:
            decoded = false;
            break;
    }
    if (decoded) {
        buf[pos++] = c;
    }
    return decoded;
}

bool MessageParser::wasMessageDecoded() {
    return state == END_OF_MESSAGE_MARK_PARSED;
}

MessageParser::State MessageParser::getState() {
    return state;
}

bool MessageParser::isReceivingMessage() {
    return (state
            & (START_OF_MESSAGE_MARK_PARSED | ID_PARSED | TYPE_PARSED | FLAGS_PARSED | PAYLOAD_LENGTH_PARSED
                    | PAYLOAD_PARSED)) > 0;
}

bool MessageParser::collectDecodedMessage(Message *message) {
    if (wasMessageDecoded()) {
        message->setId(buf[Message::ID_POS]);
        message->setType(buf[Message::TYPE_POS]);
        message->setFlags(buf[Message::FLAGS_POS]);
        message->setPayloadSize(buf[Message::PAYLOAD_LENGTH_POS]);
        unsigned char* messagePayload = message->getPayload();
        memcpy((void *) messagePayload, (const void *) &(buf[Message::PAYLOAD_POS]), buf[Message::PAYLOAD_LENGTH_POS]);
        reset();
        return true;
    }
    return false;
}

unsigned char MessageParser::getMaxPayloadLength() {
    return maxPayloadLength;
}

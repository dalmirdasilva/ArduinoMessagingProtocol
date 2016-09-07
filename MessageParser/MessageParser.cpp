#include "MessageParser.h"
#include <Message.h>
#include <string.h>
#include <Arduino.h>

MessageParser::MessageParser(unsigned char *buf, unsigned short len)
        : buf(buf), len(len), pos(0) {
    reset();
}

void MessageParser::reset() {
    state = INITIAL;
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
    if (pos >= len) {
        decoded = false;
    } else {
        switch (state) {
        case INITIAL:
            if (c == Message::START) {
                pos = 0;
                crc = 0;
                crcMismatch = false;
                state = START_OF_MESSAGE_MARK_PARSED;
            } else {
                decoded = false;
            }
            break;
        case START_OF_MESSAGE_MARK_PARSED:
            state = ID_PARSED;
            crc += c;
            break;
        case ID_PARSED:
            state = TYPE_PARSED;
            crc += c;
            break;
        case TYPE_PARSED:
            state = PAYLOAD_LENGTH_PARSED;
            payloadLength = c;
            if (payloadLength > 0) {
                state = PAYLOAD_LENGTH_PARSED;
            } else {
                state = PAYLOAD_PARSED;
            }
            crc += c;
            break;
        case PAYLOAD_LENGTH_PARSED:
            if ((pos - Message::PAYLOAD_POS) + 1 >= payloadLength) {
                state = PAYLOAD_PARSED;
            }
            crc += c;
            break;
        case PAYLOAD_PARSED:
            state = CRC_PARSED;
            if (crc != c) {
                crcMismatch = true;
            }
            break;
        case CRC_PARSED:
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
    }
    return decoded;
}

bool MessageParser::wasMessageDecoded() {
    return state == END_OF_MESSAGE_MARK_PARSED;
}

MessageParser::State MessageParser::getState() {
    return state;
}

bool MessageParser::getDecodedMessage(Message *message) {
    if (wasMessageDecoded()) {
        message->setId(buf[Message::ID_POS]);
        message->setType(buf[Message::TYPE_POS]);
        message->setPayloadSize(buf[Message::PAYLOAD_LENGTH_POS]);
        unsigned char* messagePayload = message->getPayload();
        memcpy((void *) messagePayload, (const void *) &(buf[Message::PAYLOAD_POS]), buf[Message::PAYLOAD_LENGTH_POS]);
        message->setCrcMismatch(crcMismatch);
        reset();
        return true;
    }
    return false;
}

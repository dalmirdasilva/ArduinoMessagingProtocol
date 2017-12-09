/**
 * Message
 */

#ifndef __ARDUINO_MESSAGING_PROTOCOL_MESSAGE_H__
#define __ARDUINO_MESSAGING_PROTOCOL_MESSAGE_H__ 1

/**
 * Message raw components:
 * [
 *  START_OF_MESSAGE_MARK {1}
 *  ID {1}
 *  TYPE {1}
 *  FLAGS {1}
 *  PAYLOAD_LENGTH {1}
 *  PAYLOAD {PAYLOAD_LENGTH}
 *  END_OF_MESSAGE_MARK {1}
 * ]
 */
class Message {

    unsigned char id;
    unsigned char type;
    unsigned char flags;
    unsigned char payloadSize;
    unsigned char* payload;

public:

    enum Type {
        DATA = 0x00, ACK = 0x01, CONNECT = 0x02, SYNC = 0x03, EPOCH = 0x04, SETTINGS = 0x05, PING = 0x06, PONG = 0x07, GENERIC_RESPONSE = ACK | CONNECT | PONG, ANY = 0xff
    };

    enum Mark {
        START = 0xaa, END = 0xbb,
    };

    enum Position {
        ID_POS = 0x01, TYPE_POS = 0x02, FLAGS_POS = 0x03, PAYLOAD_LENGTH_POS = 0x04, PAYLOAD_POS = 0x05
    };

    enum Flag {
        REQUIRED_ACK = 0x01, IS_LAST_MESSAGE = 0x02
    };

    static unsigned char nextId;

    Message();

    Message(unsigned char* payload);

    Message(unsigned char id, unsigned char type, unsigned char flags, unsigned char payloadSize,
            unsigned char* payload);

    void reset();

    void generateNextId();

    unsigned char getId();

    void setId(unsigned char id);

    unsigned char getType();

    void setType(unsigned char type);

    unsigned char getFlags();

    void setFlags(unsigned char flags);

    void clearFlags();

    void addFlags(unsigned char flags);

    void removeFlags(unsigned char flags);

    unsigned char getPayloadSize();

    void setPayloadSize(unsigned char payloadsize);

    unsigned char* getPayload();

    void setPayload(unsigned char* payload);

    unsigned int toRaw(unsigned char* raw);
};

#endif // __ARDUINO_MESSAGING_PROTOCOL_MESSAGE_H__

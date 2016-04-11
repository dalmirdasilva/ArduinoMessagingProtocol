/**
 * Message
 */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__ 1

/**
 * Message raw components:
 * [
 *  START_OF_MESSAGE_MARK {1}
 *  ID {1}
 *  TYPE {1}
 *  PAYLOAD_LENGTH {1}
 *  PAYLOAD {PAYLOAD_LENGTH}
 *  END_OF_MESSAGE_MARK {1}
 * ]
 */
class Message {

    unsigned char id;
    unsigned char type;
    unsigned char payloadSize;
    unsigned char* payload;

public:

    enum Type {
        DATA, ACK, CONNECT, SYNC, EPOCH, SETTINGS, PING, PONG
    };

    enum Mark {
        START = 0xaa, END = 0xbb,
    };

    enum Position {
        ID_POS = 1, TYPE_POS = 2, PAYLOAD_LENGTH_POS = 3, PAYLOAD_POS = 4
    };

    static unsigned char nextId;

    Message();

    Message(unsigned char* payload);

    Message(unsigned char id, unsigned char type, unsigned char payloadSize, unsigned char* payload);

    void reset();

    unsigned char getId();

    void setId(unsigned char id);

    void generateNextId();

    unsigned char getType();

    void setType(unsigned char type);

    unsigned char getPayloadSize();

    void setPayloadSize(unsigned char payloadsize);

    unsigned char* getPayload();

    void setPayload(unsigned char* payload);

    unsigned int toRaw(unsigned char* raw);
};

#endif // __MESSAGE_H__

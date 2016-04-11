/**
 * Message Parser
 */

#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__ 1

#include <Message.h>

class MessageParser {

public:
    enum State {
        INITIAL,
        START_OF_MESSAGE_MARK_PARSED,
        ID_PARSED,
        TYPE_PARSED,
        PAYLOAD_LENGTH_PARSED,
        PAYLOAD_PARSED,
        END_OF_MESSAGE_MARK_PARSED
    };

    unsigned char* buf;
    unsigned short len;
    unsigned short pos;
    unsigned char payloadLength;
    State state;

    MessageParser(unsigned char *buf, unsigned short len);

    void reset();

    short decodeBytes(unsigned char* buf, unsigned short len);

    bool decode(unsigned char c);

    bool wasMessageDecoded();

    State getState();

    bool getDecodedMessage(Message *message);
};

#endif // __MESSAGE_PARSER_H__

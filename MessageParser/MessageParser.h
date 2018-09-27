/**
 * Message Parser
 */

#ifndef __MESSAGE_PARSER_H__
#define __MESSAGE_PARSER_H__ 1

#include <Message.h>

class MessageParser {

public:
    enum State {
        INITIAL = 0x00,
        START_OF_MESSAGE_MARK_PARSED = 0x01,
        ID_PARSED = 0x02,
        TYPE_PARSED = 0x04,
        FLAGS_PARSED = 0x08,
        PAYLOAD_LENGTH_PARSED = 0x10,
        PAYLOAD_PARSED = 0x20,
        END_OF_MESSAGE_MARK_PARSED = 0x40
    };

    unsigned char* buf;
    unsigned short len;
    unsigned short pos;
    unsigned char maxPayloadLength;
    unsigned char payloadLength;
    State state;

    /**
     * Constructor.
     *
     * @param   buf A array of chars where decoded input will be placed while decoding a message.
     * @param   len The size of the array.
     */
    MessageParser(unsigned char *buf, unsigned short len);

    /**
     * Resets the state machine to the INITIAL state.
     *
     * Must be called when some input wasn't decoded (the machine state is in a invalid state).
     *
     * It is internally called after a decoded message is collected.
     */
    void reset();

    /**
     * Feeds the state machine with an array of chars.
     *
     * The as calling 'decode' with each chars from the array.
     *
     * @param   buf The array of chars.
     * @param   len The size of the array.
     * @return  short   The number of decoded chars, will be the same as len when succeed, less otherwise.
     */
    short decodeBytes(unsigned char* buf, unsigned short len);

    /**
     * Decodes a single char.
     * Feeds the decoder state machine with the char. Moves to the next state if necessary.
     *
     * @param   unsigned char   The next char received from the medium.
     * @return  bool            True if char was decoded, false if the char made the state machine to get into
     *                          a invalid state.
     */
    bool decode(unsigned char c);

    /**
     * Checks if a message was already decoded. Must be called right after 'decode'.
     *
     * @return  bool    True if the message was entirely decoded so far, false otherwise.
     */
    bool wasMessageDecoded();

    /**
     * Current state of the parser state machine.
     *
     * @return  enum    State.
     */
    State getState();

    /**
     * Returns true if parser in a state different than END_OF_MESSAGE_MARK_PARSED.
     *
     * @return  bool    True if is receiving message.
     */
    bool isReceivingMessage();

    /**
     * Collects the last decoded message if any.
     *
     * @param   message A previously-allocated message pointer where the decoded message will be stored.
     * @return  bool    True if collection succeed, false otherwise (message wasn't decoded yet)
     */
    bool collectDecodedMessage(Message *message);

    /**
     * Return max payload length.
     *
     * Max payload is the len of the buffer minus the message control len.
     *
     * @return unsigned char
     */
    unsigned char getMaxPayloadLength();
};

#endif // __MESSAGE_PARSER_H__

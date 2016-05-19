/**
 * Message Exchanger
 */

#ifndef __MESSAGE_EXCHANGER_H__
#define __MESSAGE_EXCHANGER_H__ 1

#include <OutputStream.h>
#include <InputStream.h>
#include <MessageParser.h>
#include <Message.h>
#include <Notifier.h>

#define INFINITY_TIMEOUT                0
#define CONNECTION_TRY_TIMEOUT          100
#define MESSAGE_SENT_ACK_TIMEOUT        1000
#define IS_CONNECTED_TIMEOUT            1000

class RemoteDevice {

    MessageParser* parser;

    Message* message;

    OutputStream* outputStream;

    InputStream* inputStream;

public:

    /**
     * Public constructor.
     *
     * @param parser        The message parser user to decode incoming messages.
     * @param message       A instance of message, used for internal methods e.q. isConnected and connect.
     *                      This message would be reseted at any time. If you are using the instance elsewere be
     *                      aware that it could be changed when you call an internal method.
     * @param outputStream   A input stream to the device.
     * @param outputStream   A output stream to the device.
     */
    RemoteDevice(MessageParser* parser, Message *message, OutputStream *outputStream, InputStream *inputStream);

    /**
     * Virtual destructor.
     */
    virtual ~RemoteDevice();

    /**
     * Transmit a message
     */
    bool transmitMessage(Message *message);

    /**
     * Try to receive a message
     */
    bool receiveMessage(Message* message);

    /**
     * Executes a connect operation with the device.
     *
     * It sends a connect message and waits for the same type of message to
     * arrive.
     *
     * Busy wait!
     */
    bool connect(unsigned long timeout);

    /**
     * Checks if the bluetooth module is connected to the host.
     */
    bool isConnected(unsigned long timeout);

    /**
     * Waits for a specific type of message to come.
     *
     * @param type      The type of the message we are waiting for.
     *                  Busy wait!
     */
    bool waitForMessageType(Message::Type type, unsigned long timeout);

    /**
     * Transmit a message and waits a new one with the given type to arrive.
     */
    bool transmitWaitingForMessageType(Message *message, Message::Type type, unsigned long timeout);
};

#endif // __MESSAGE_EXCHANGER_H__

/**
 * Notifier
 */

#ifndef __ARDUINO_MESSAGING_PROTOCOL_NOTIFIER_H__
#define __ARDUINO_MESSAGING_PROTOCOL_NOTIFIER_H__ 1



class Notifier {

    static unsigned char transmitingPin;
    static unsigned char waitingPin;

public:

    /**
     * Setups the notifier.
     *
     * @param   transmitingPin  The pin that will be set to HIGH level when the transmission starts.
     * @param   waitingPin      The pin that will be set to HIGH level when it starts to wait form a message.
     */
    static void setup(unsigned char transmitingPin, unsigned char waitingPin);

    /**
     * Called when start transmission.
     */
    static void notifyBeginTransmiting();

    /**
     * Called when end transmission.
     */
    static void notifyEndTransmiting();

    /**
     * Called when start waiting.
     */
    static void notifyBeginWaiting();

    /**
     * Called when finish waiting.
     */
    static void notifyEndWaiting();
};

#endif // __ARDUINO_MESSAGING_PROTOCOL_NOTIFIER_H__

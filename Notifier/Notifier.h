/**
 * Notifier
 */

#ifndef __NOTIFIER_H__
#define __NOTIFIER_H__ 1

class Notifier {

public:

    static void notifyTransmiting();

    static void unnotifyTransmiting();

    static void notifyWaiting();

    static void unnotifyWaiting();
};

#endif // __NOTIFIER_H__

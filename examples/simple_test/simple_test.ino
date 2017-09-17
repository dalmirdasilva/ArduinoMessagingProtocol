
#include <Arduino.h>
#include <SeekableOutputStream.h>
#include <ByteArrayInputStream.h>
#include <ByteArrayOutputStream.h>
#include <Message.h>
#include <MessageParser.h>
#include <RemoteDevice.h>
#include <Notifier.h>

#define BUFF_SIZE               64
#define TRANSMITING_LED_PIN     13
#define WAITING_LED_PIN         11

unsigned char inputArray[BUFF_SIZE];
unsigned char outputArray[BUFF_SIZE];
unsigned char messagePayload[BUFF_SIZE];
unsigned char parserBuffer[BUFF_SIZE];

ByteArrayInputStream is = ByteArrayInputStream(inputArray, BUFF_SIZE);
ByteArrayOutputStream os = ByteArrayOutputStream(outputArray, BUFF_SIZE);
Message message = Message(messagePayload);
MessageParser parser = MessageParser(parserBuffer, BUFF_SIZE);
RemoteDevice device = RemoteDevice(&parser, &message, &os, &is);

void dumpArray(unsigned char *array, unsigned short size) {
    for(unsigned int i = 0; i < size; i++) {
        Serial.print("0x");
        Serial.print(array[i], HEX);
        Serial.print(", ");
    }
    Serial.println();
}

void assert(bool exp, int line) {
    if (exp) {
        Serial.print("Passed: ");
    } else {
        Serial.print("Failed: ");
    }
    Serial.println(line);
}

void testSendMessage() {
    unsigned char id = 0x01, flags = 0x02, payloadSize = 0x03, randomByte = 0xdd;
    message.reset();
    message.setId(id);
    message.setType(Message::Type::DATA);
    message.setFlags(flags);
    message.setPayloadSize(payloadSize);
    unsigned char *payload = message.getPayload();
    payload[0] = randomByte;
    dumpArray(inputArray, BUFF_SIZE);
    device.transmitMessage(&message);
    dumpArray(outputArray, BUFF_SIZE);
    assert(outputArray[0] == Message::Mark::START, __LINE__);
    assert(outputArray[1] == id, __LINE__);
    assert(outputArray[2] == Message::Type::DATA, __LINE__);
    assert(outputArray[3] == flags, __LINE__);
    assert(outputArray[4] == payloadSize, __LINE__);
    assert(outputArray[5] == randomByte, __LINE__);
    assert(outputArray[8] == Message::Mark::END, __LINE__);
}

void testReceiveMessage() {
    unsigned char id = 0x02, flags = 0x03, payloadSize = 0x02, randomByte = 0xff;
    inputArray[0] = Message::Mark::START;
    inputArray[1] = id;
    inputArray[2] = Message::Type::DATA;
    inputArray[3] = flags;
    inputArray[4] = payloadSize;
    inputArray[5] = randomByte;
    inputArray[7] = Message::Mark::END;
    message.reset();
    is.reset();
    assert(device.receiveMessage(&message), __LINE__);
    unsigned char *payload = message.getPayload();
    payload[0] = randomByte;
    assert(message.getId() == id, __LINE__);
    assert(message.getType() == Message::Type::DATA, __LINE__);
    assert(message.getFlags() == flags, __LINE__);
    assert(message.getPayloadSize() == payloadSize, __LINE__);
    message.setType(Message::Type::DATA);
    message.setFlags(flags);
    message.setPayloadSize(payloadSize);
}

void setup() {
    Serial.begin(9600);
    Serial.println("Initializing...");
    Notifier::setup(TRANSMITING_LED_PIN, WAITING_LED_PIN);
    testSendMessage();
    testReceiveMessage();
    Serial.println("Done");
}

void loop() {
}
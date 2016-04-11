## Arduino Messaging Protocol

The objective of this project is provide a minimal messaging protocol to 
connect to exchange serial messages between an Arduino device and any other more powerful 
computer, such as: Android smartphones, Raspberry PI, PC, etc.

## Protocol components

### Message
A message is the highest level object exchanged between parts. A message has the following structure:
```
Message {
	START_OF_MESSAGE_MARK {1*}
	ID {1*}
	TYPE {1*}
	PAYLOAD_LENGTH {1*}
	PAYLOAD {PAYLOAD_LENGTH*}
	END_OF_MESSAGE_MARK {1*}
}
* = field length in bytes
```
A message could carry up to 255 bytes.

### Depends on:
- ArduinoIO <git@github.com:dalmirdasilva/ArduinoIO.git>
ArduinoIO provides the InputStream and OutputStream concept. Both are implemented by
some specific I/O in Arduino, such as: Serial, i2c, memory, etc.

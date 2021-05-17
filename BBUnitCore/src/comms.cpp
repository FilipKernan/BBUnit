#include "comms.h"
#include <Arduino.h>

Comms::Comms(int tx, int rx, int baud)
{
    serial = new SoftwareSerial(rx, tx);

    baudRate = baud;

}

Comms::~Comms()
{
    free(serial);
}

void Comms::init() {
    serial->begin(baudRate);
}

// TODO: change to use write instead of print, also define packet
int Comms::write(char* buff) {
    int msgSize = 10;
    if (serial->availableForWrite() <= msgSize) {
        serial->write(buff, msgSize);
        return 0;
    }
    return -1;
}


String Comms::read() {
    serial->listen();
    int size = serial->available();
    if (size) {
        char msg[size + 1];
        for (int i = 0; i<10; i++) {
            msg[i] = serial->read();
        }
        return msg;
    }
    return "";
    
}

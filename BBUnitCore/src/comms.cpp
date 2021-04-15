#include "comms.h"

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
int Comms::write(String msg) {
    int msgSize = msg.length() + 1;
    if (serial->availableForWrite() <= msgSize) {
        serial->print(msg);
        return 0;
    }
    return -1;
}


String Comms::read() {
    int size = serial->available();
    if (size) {
        char msg[size + 1];
        for (int i = 0; serial->available(); i++) {
            msg[i] = serial->read();
        }
        msg[size] = 0;
        return msg;
    }
    return "";
    
}

#include <SoftwareSerial.h>
#include <string.h>

class Comms
{
private:
    int tx;
    int rx;
    SoftwareSerial* serial;
    int baudRate;
public:
    int write(char*); // null terminated
    String read();
    void init();
    Comms(int tx, int rx, int baud);
    ~Comms();
    
};

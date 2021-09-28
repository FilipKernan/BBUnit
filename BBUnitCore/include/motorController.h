#include <Arduino.h>

class MotorController
{
private:
    int dirPin, pwmPin;
    uint8_t num;
    volatile long* encoderCount;
public:
    MotorController(int dir, int pwm, uint8_t num, volatile long* encoderCount); // needs dir port, pwm port
    ~MotorController();
    void write(int direction, int pwmSpeed);
    long getEncoderCount();
    char* encoderToComms();
};
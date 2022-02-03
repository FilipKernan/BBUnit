#include <Arduino.h>

class MotorController
{
private:
    int dirPin, pwmPin;
    uint8_t num;
    long goal;
    float kp, ki, kd;
    volatile long* encoderCount;
    uint8_t effort;
    int integral[10];
    uint8_t intergalIndex;
    unsigned long timeDeltas[10];
    unsigned long lastTime;
    bool usePid;
public:
    MotorController(int dir, int pwm, uint8_t num, volatile long* encoderCount, float kp, float ki, float kd, bool usePid, int forward); // needs dir port, pwm port
    ~MotorController();
    void write(int direction, int pwmSpeed);
    long getEncoderCount();
    char* encoderToComms();
    void updatePID(float kp, float ki, float kd);
    void updateMotor();
    void setGoal(long goal);
    long getGoal();

};
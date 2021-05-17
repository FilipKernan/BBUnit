class MotorController
{
private:
    int dirPin, pwmPin;
    uint8_t num;
    long* encoderCount;
public:
    MotorController(int dir, int pwm, uint8_t num, long* encoderCount); // needs dir port, pwm port
    ~MotorController();
    void write(int direction, int pwmSpeed);
    long getEncoderCount();
    char* encoderToComms();
};
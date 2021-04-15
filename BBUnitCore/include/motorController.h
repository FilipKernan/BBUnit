class MotorController
{
private:
    int dirPin, pwmPin;
public:
    MotorController(int dir, int pwm); // needs dir port, pwm port
    ~MotorController();
    void write(int direction, int pwmSpeed);
};
// interupt pins 18, 19, 2, 3



#define LEFT_MOTOR_DIR 53
#define LEFT_MOTOR_PWM 10
#define LEFT_MOTOR_INT 3
#define LEFT_MOTOR_A 24
#define LEFT_MOTOR_B 25
#define LEFT_DIR -1
volatile long leftEncoderCount = 0;
volatile bool lastLeftA = 0;
volatile bool lastLeftB = 0;


#define RIGHT_MOTOR_DIR 50
#define RIGHT_MOTOR_PWM 9
#define RIGHT_MOTOR_INT 18
#define RIGHT_MOTOR_A 27
#define RIGHT_MOTOR_B 26
volatile long rightEncoderCount = 0;
volatile bool lastRightA = 0;
volatile bool lastRightB = 0;


#define FRONT_MOTOR_DIR 51
#define FRONT_MOTOR_PWM 11
#define FRONT_MOTOR_INT 2
#define FRONT_MOTOR_A 23
#define FRONT_MOTOR_B 22
volatile long frontEncoderCount = 0;
volatile bool lastFrontA = 0;
volatile bool lastFrontB = 0;


#define BACK_MOTOR_DIR 52
#define BACK_MOTOR_PWM 5
#define BACK_MOTOR_INT 19
#define BACK_MOTOR_A 28
#define BACK_MOTOR_B 29
volatile long backEncoderCount = 0;
volatile bool lastBackA = 0;
volatile bool lastBackB = 0;
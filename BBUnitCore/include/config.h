// interupt pins 18, 19, 2, 3
#include <esp_task_wdt.h>


#define LEFT_MOTOR_DIR 53
#define LEFT_MOTOR_PWM 9
#define LEFT_MOTOR_INT 18 
#define LEFT_MOTOR_A 26
#define LEFT_MOTOR_B 27
#define LEFT_DIR -1
volatile long leftEncoderCount = 0;
volatile bool lastLeftA = 0;
volatile bool lastLeftB = 0;
void leftEncoderChange();


#define RIGHT_MOTOR_DIR 50
#define RIGHT_MOTOR_PWM 45
#define RIGHT_MOTOR_INT 3
#define RIGHT_MOTOR_A 24
#define RIGHT_MOTOR_B 24
volatile long rightEncoderCount = 0;
volatile bool lastRightA = 0;
volatile bool lastRightB = 0;
void rightEncoderChange();


#define FRONT_MOTOR_DIR 51
#define FRONT_MOTOR_PWM 52
#define FRONT_MOTOR_INT 19
#define FRONT_MOTOR_A 28
#define FRONT_MOTOR_B 29
volatile long frontEncoderCount = 0;
volatile bool lastFrontA = 0;
volatile bool lastFrontB = 0;
void frontEncoderChange();

#define BACK_MOTOR_DIR 51
#define BACK_MOTOR_PWM 44
#define BACK_MOTOR_INT 2
#define BACK_MOTOR_A 23
#define BACK_MOTOR_B 22
volatile long backEncoderCount = 0;
volatile bool lastBackA = 0;
volatile bool lastBackB = 0;
void backEncoderChange();

const char* ssid     = "BR-95";
const char* password = "StormBlessed";

#define BUFFER_SIZE 32


void RobotTaskLoop(void* paramaters);
void CommsTaskLoop(void* paramaters);

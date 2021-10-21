// interupt pins 18, 19, 2, 3

#include <esp_task_wdt.h>
#ifndef CONFIG
#define CONFIG 



#define LEFT_MOTOR_DIR 53
#define LEFT_MOTOR_PWM 9
#define LEFT_MOTOR_INT 18 
#define LEFT_MOTOR_A 26
#define LEFT_MOTOR_B 27
#define LEFT_DIR -1
extern volatile long leftEncoderCount;
extern volatile bool lastLeftA;
extern volatile bool lastLeftB;
void leftEncoderChange();


#define RIGHT_MOTOR_DIR 50
#define RIGHT_MOTOR_PWM 45
#define RIGHT_MOTOR_INT 3
#define RIGHT_MOTOR_A 24
#define RIGHT_MOTOR_B 24
extern volatile long rightEncoderCount;
extern volatile bool lastRightA;
extern volatile bool lastRightB;
void rightEncoderChange();


#define FRONT_MOTOR_DIR 51
#define FRONT_MOTOR_PWM 52
#define FRONT_MOTOR_INT 19
#define FRONT_MOTOR_A 28
#define FRONT_MOTOR_B 29
extern volatile long frontEncoderCount;
extern volatile bool lastFrontA;
extern volatile bool lastFrontB;
void frontEncoderChange();

#define BACK_MOTOR_DIR 51
#define BACK_MOTOR_PWM 44
#define BACK_MOTOR_INT 2
#define BACK_MOTOR_A 23
#define BACK_MOTOR_B 22
extern volatile long backEncoderCount;
extern volatile bool lastBackA;
extern volatile bool lastBackB;
void backEncoderChange();



#define BUFFER_SIZE 16


void RobotTaskLoop(void* paramaters);
void CommsTaskLoop(void* paramaters);


static SemaphoreHandle_t commsMutex;

static SemaphoreHandle_t writeMutex;
static SemaphoreHandle_t readMutex;


#endif
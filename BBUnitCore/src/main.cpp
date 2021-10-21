#include <Arduino.h>
#include "config.h"
#include "comms.h"
#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
// #include "motorController.h"


// MotorController* leftMotor;
// MotorController* rightMotor;
Comms* comms;
TaskHandle_t CommsTask;
TaskHandle_t RobotTask;
const int baudrate = 115200;
  
unsigned int targetTime = 0;

uint8_t motorPower = 0;




volatile long leftEncoderCount = 0;
volatile bool lastLeftA = 0;
volatile bool lastLeftB = 0;

volatile long rightEncoderCount = 0;
volatile bool lastRightA = 0;
volatile bool lastRightB = 0;

volatile long frontEncoderCount = 0;
volatile bool lastFrontA = 0;
volatile bool lastFrontB = 0;

volatile long backEncoderCount = 0;
volatile bool lastBackA = 0;
volatile bool lastBackB = 0;

const char* ssid     = "BR-95";
const char* password = "StormBlessed";

WiFiServer wifiServer(80);

/***
 * TODO: 
 * Today
 * 		Test encoder output
 * 		test the bluetooth
 * Tommorow
 * 		set up packets
 * 		Read from gyro
 * 		
 * 
 */
// DigitalPin<FRONT_MOTOR_A> frontA;
// DigitalPin<FRONT_MOTOR_B> frontB;

// DigitalPin<RIGHT_MOTOR_A> rightA;
// DigitalPin<RIGHT_MOTOR_B> rightB;

// DigitalPin<BACK_MOTOR_A> backA;
// DigitalPin<BACK_MOTOR_B> backB;

// DigitalPin<LEFT_MOTOR_A> leftA;
// DigitalPin<LEFT_MOTOR_B> leftB;

void setup() {



	// put your setup code here, to run once:
	// leftMotor = new MotorController(LEFT_MOTOR_DIR, LEFT_MOTOR_PWM, 3, &leftEncoderCount);
	// rightMotor = new MotorController(RIGHT_MOTOR_DIR, RIGHT_MOTOR_PWM, 1, &rightEncoderCount);

	comms = new Comms();
	commsMutex = xSemaphoreCreateMutex();
	writeMutex = xSemaphoreCreateMutex();
	readMutex = xSemaphoreCreateMutex();
	Serial.begin(9600);

	Serial.println("booting up");
	targetTime = millis();
	// WiFi.begin();
	WiFi.softAP(ssid, password);

	IPAddress IP = WiFi.softAPIP();

	wifiServer.begin();
	
	ArduinoOTA.setHostname("br95");
	ArduinoOTA.setPassword("StormBlessed");



	ArduinoOTA
    	.onStart([]() {
			String type;
			if (ArduinoOTA.getCommand() == U_FLASH)
				type = "sketch";
			else // U_SPIFFS
				type = "filesystem";

			// NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
			Serial.println("Start updating " + type);
		})
		.onEnd([]() {
			Serial.println("\nEnd");
		})
		.onProgress([](unsigned int progress, unsigned int total) {
			Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
		})
		.onError([](ota_error_t error) {
			Serial.printf("Error[%u]: ", error);
			if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
			else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
			else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
			else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
			else if (error == OTA_END_ERROR) Serial.println("End Failed");
		});

  	ArduinoOTA.begin();


	Serial.println("Ready");
	Serial.print("IP address: ");
	Serial.println(IP);

	// attachInterrupt(digitalPinToInterrupt(FRONT_MOTOR_INT), frontEncoderChange, CHANGE);
	// frontA.mode(INPUT);
	// frontB.mode(INPUT);

	// attachInterrupt(digitalPinToInterrupt(RIGHT_MOTOR_INT), rightEncoderChange, CHANGE);
	// rightA.mode(INPUT);
	// rightB.mode(INPUT);

	// attachInterrupt(digitalPinToInterrupt(BACK_MOTOR_INT), backEncoderChange, CHANGE);
	// backA.mode(INPUT);
	// backB.mode(INPUT);

	// attachInterrupt(digitalPinToInterrupt(LEFT_MOTOR_INT), leftEncoderChange, CHANGE);
	// leftA.mode(INPUT);
	// leftB.mode(INPUT);


	

	// create tasks

	  //create a task that will be executed in the Task1code() function, with priority 1 and executed on core 0
	xTaskCreatePinnedToCore(
						CommsTaskLoop,   /* Task function. */
						"CommsTask",     /* name of task. */
						10000,       /* Stack size of task */
						NULL,        /* parameter of the task */
						1,           /* priority of the task */
						&CommsTask,      /* Task handle to keep track of created task */
						0);          /* pin task to core 0 */                  
	delay(500); 

	//create a task that will be executed in the Task2code() function, with priority 1 and executed on core 1
	xTaskCreatePinnedToCore(
						RobotTaskLoop,   /* Task function. */
						"RobotTask",     /* name of task. */
						10000,       /* Stack size of task */
						NULL,        /* parameter of the task */
						1,           /* priority of the task */
						&RobotTask,      /* Task handle to keep track of created task */
						1);          /* pin task to core 1 */
	delay(500); 

	esp_task_wdt_init(2000000, 0);
	esp_task_wdt_add(CommsTask);
	esp_task_wdt_add(RobotTask);


}

void RobotTaskLoop(void* paramaters) {
	while(true) {
		ArduinoOTA.handle();
		// comms test
		xSemaphoreTake(commsMutex, portMAX_DELAY);

		if (comms->newData){
			// Serial.write(comms->read(), BUFFER_SIZE);
		}
		

		xSemaphoreGive(commsMutex);

		char toWrite[BUFFER_SIZE] = {'A', 'l', 'i', 'v', 'e', 'A', 'n', 'd', 'K', 'i', 'c', 'k', 'i', 'n', 'g', '!'};
		xSemaphoreTake(commsMutex, portMAX_DELAY);

		comms->write(toWrite);
		xSemaphoreGive(commsMutex);
		esp_task_wdt_reset();
	}
}


void CommsTaskLoop(void* paramaters) {
	while(true) {
		
		WiFiClient client = wifiServer.available();
	
		if (client){
			// this while loop is nessary, have it run on the second core so that it does not harm everything
			while (client.connected()) {
				// Serial.println("connected to client");
				// read data from wifi client and send to serial
				xSemaphoreTake(commsMutex, portMAX_DELAY);

				comms->relay(&client);
				xSemaphoreGive(commsMutex);

				esp_task_wdt_reset();
			
				
			}
		}
			
		client.stop();
		esp_task_wdt_reset();
	}
}


void loop() {
	// char* readPointer = comms->read();
	// if (readPointer != NULL) {
	// 	Serial.println("packet detected, writing to motors");
	// 	Serial.println(readPointer);
	// 	// rightMotor->write(LOW, *(readPointer + 3));
	// 	// leftMotor->write(HIGH, *(readPointer + 7));
	// 	// targetTime = millis() + 10 * (*(readPointer + 2));

	// }
	


	// wait for client
	// Serial.println("waiting for client");
  	
	// while (comms->serial->available())	{
	// 	String power = comms->serial->readString();
	// 	Serial.println(power);
	// 	int powerInt = power.toInt();
	// 	Serial.println(powerInt);
	// 	// rightMotor->write(LOW, powerInt);
	// 	// leftMotor->write(HIGH, powerInt);
	// 	targetTime = millis() + 1000;
	// }
	// comms->write("I am stormblessed");
	// Serial.println(rightMotor->getEncoderCount());
	// Serial.println(leftMotor->getEncoderCount());


}



// void leftEncoderChange() {
// 	bool a = fastDigitalRead(LEFT_MOTOR_A);
// 	bool b = fastDigitalRead(LEFT_MOTOR_B);

// 	leftEncoderCount += (a ^ lastLeftB) - (lastLeftA ^ b);

// 	lastLeftA = a;
// 	lastLeftB = b;
	
// }

// void rightEncoderChange() {
// 	bool a = fastDigitalRead(RIGHT_MOTOR_A);
// 	bool b = fastDigitalRead(RIGHT_MOTOR_B);

// 	rightEncoderCount += (a ^ lastRightB) - (lastRightA ^ b);

// 	lastRightA = a;
// 	lastRightB = b;
	
// }

// void frontEncoderChange() {
// 	bool a = fastDigitalRead(FRONT_MOTOR_A);
// 	bool b = fastDigitalRead(FRONT_MOTOR_B);

// 	frontEncoderCount += (a ^ lastFrontB) - (lastFrontA ^ b);

// 	lastFrontA = a;
// 	lastFrontB = b;
	
// }

// void backEncoderChange() {
// 	bool a = fastDigitalRead(BACK_MOTOR_A);
// 	bool b = fastDigitalRead(BACK_MOTOR_B);

// 	backEncoderCount += (a ^ lastBackB) - (lastBackA ^ b);

// 	lastBackA = a;
// 	lastBackB = b;
	
// }
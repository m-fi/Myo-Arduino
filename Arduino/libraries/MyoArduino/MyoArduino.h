/**
 * @file   MyoArduino.h
 * @author Matt Fielder
 * @date   December 2017 - March 2018
 * @brief  Header file for MyoArduino output operations.
 */
 
#ifndef MyoArduino_h //MyoArduino_h
#define MyoArduino_h

#include "Arduino.h"
#include "LiquidCrystal.h"
#include "math.h"
#include "Servo.h"

class Output
{
	public:
	int poseNumber(String poseName);
	void changeLedState(int pinNumber);
	void outputMessages(String messageText, LiquidCrystal& lcd);
	void outputMessages(String messageTextTop, String messageTextBottom, LiquidCrystal& lcd);
	void setServoPosition(Servo& servo, int angleChange);
	
};



#define MyoArduino_Functions
#include "MyoArduino.cpp"

#endif //MyoArduino_h
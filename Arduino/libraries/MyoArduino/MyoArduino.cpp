/**
 * @file   MyoArduino.cpp
 * @author Matt Fielder
 * @date   December 2017 - March 2018
 * @brief  Implementation file for MyoArduino output operations.
 */
 
#ifndef MyoArduino_Functions

#include "MyoArduino.h"
#include "Arduino.h"
#include "LiquidCrystal.h"
#include "math.h"

//assign the pose a number
//this allows us to easily control outputs based on the pose
int Output::poseNumber(String poseName)
{
	int poseNumber = 0;
	if (poseName == "MYO_POSE_REST"){poseNumber = 1;}
	else if (poseName == "MYO_POSE_FIST"){poseNumber = 2;}
	else if (poseName == "MYO_POSE_WAVE_IN"){poseNumber = 3;}
	else if (poseName == "MYO_POSE_WAVE_OUT"){poseNumber = 4;}
	else if (poseName == "MYO_POSE_FINGERS_SPREAD"){poseNumber = 5;}
	else if (poseName == "MYO_POSE_DOUBLE_TAP"){poseNumber = 6;}
	return poseNumber;
}

//change the on/off state of an LED
//expects the Arduino pinNumber that the LED is connected to
void Output::changeLedState(int pinNumber)
{
	//check whether the pin already has voltage or not
	int ledState = digitalRead(pinNumber);
	//store as text for serial output debugging
	String pinNumberText = String("PIN ") + pinNumber;

	//if 0V(off) then set to 5V(on) 
	if (ledState == LOW) 
	{
		digitalWrite(pinNumber, HIGH);
		Serial.println(pinNumberText + String(" turned ON")); //turn on
	} 
    //ELSE change from 5V(on) to 0V(off)
	else 
	{
		digitalWrite(pinNumber, LOW);
		Serial.println(pinNumberText + String(" turned OFF")); //turn off
	}

}

//outputs a message to both the Serial port and the LCD
//LCD allows 16 characters per line (16 x 2)
void Output::outputMessages(String messageText, LiquidCrystal& lcd)
{
	//output to Serial port
	Serial.println(messageText);
	
	//clear LCD
	lcd.clear();
	//output to LCD
	lcd.print(messageText);
}

//outputs messages to both the Serial port and the LCD
//this function allows 2 strings
//the first string is output to the top line of the LCD
//second string is output to the bottom line of the LCD
void Output::outputMessages(String messageTextTop, String messageTextBottom, LiquidCrystal& lcd)
{
	//output to Serial port
	Serial.println(messageTextTop + " " + messageTextBottom);
	
	//clear LCD
	lcd.clear();
	//output the top line
	lcd.print(messageTextTop);
	//change cursor position to bottom row of the LCD
	lcd.setCursor(0,1);
	//output the bottom line
	lcd.print(messageTextBottom);
}

//changes the position of the servo based on what angle has been received by the function
void Output::setServoPosition(Servo& servo, int angleChange)
{
	//the servo can be quite jittery when changing position, this can also happen when no servo.write() has been called
	//this is avoided by only 'attaching' the servo when it is needed (moving), otherwise we 'detach' it
	//this method is described here: https://forum.arduino.cc/index.php?topic=405092.msg3490787#msg3490787
	
	//attach the servo as it needs to change position soon
	servo.attach(9);
	
	//get the current position (the last .write() angle sent to the servo)
	int currentPosition = servo.read();
	//get the new position by adding the angleChange to the currentPosition, constrain it to the servo parameters
	int newPosition = constrain(currentPosition + angleChange, 0, 179);
	
	Serial.print("Servo current: ");Serial.print(currentPosition);Serial.print(", angleChange: ");
	Serial.print(angleChange);Serial.print(", newPosition: ");Serial.println(newPosition);
	//send new position to the servo
	servo.write(newPosition);
	//delay to let the servo move to newPosition before detaching it
	delay(500);
	//detach servo
	servo.detach();
}

#endif
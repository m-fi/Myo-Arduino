#include "MyoArduino.h"
#include "Arduino.h"

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

void Output::changeLedState(int pinNumber)
{
	//check whether the pin already has voltage or not
	int ledState = digitalRead(pinNumber);
	//store as text for serial output debugging
	String pinNumberText = String("PIN ") + pinNumber;

	//if 0V then set to 5V ELSE change from 5V to 0V
	if (ledState == LOW) 
	{
		digitalWrite(pinNumber, HIGH);
		Serial.println(pinNumberText + String("turned ON")); //turned on
	} 
	else 
	{
		digitalWrite(pinNumber, LOW);
		Serial.println(pinNumberText + String("turned OFF")); //turned off
	}

}

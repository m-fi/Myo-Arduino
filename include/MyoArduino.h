#ifndef MyoArduino_h
#define MyoArduino_h

#include "Arduino.h"

class Output
{
	public:
	int poseNumber(String poseName);
	void changeLedState(int pinNumber);
	
	private:
	//String poseName;
	
};

#endif //MyoArduino_h
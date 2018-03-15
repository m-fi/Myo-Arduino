/**
 * @file   Quaternions.h
 * @author Matt Fielder
 * @date   January - March 2018
 * @brief  Header file for quaternion operations.
 */
 
#ifndef Quaternions_h //Quaternions_h
#define Quaternions_h

#include "Arduino.h"
#include "math.h"

class Quaternion
{
public:
	//variables
	float w, x, y, z;
	String name;

	//constructor
	Quaternion();
	
	//pass and store the name of the Quaternion for later use
	void passName(String _name);

	//pass data to Quaternion
	void passQuaternion(int16_t quat[4]);

	//Norm-squared
	float normSquared();

	//multiply two Quaternions
	Quaternion operator* (const Quaternion& q2);

	//equals: assign the values of one Quaternion to another
	Quaternion operator= (const Quaternion& q);

	//conjugate: returns the conjugate of a Quaternion
	Quaternion conjugate();
  
	//return a Quaternion that contains the difference between q1 and q2
	Quaternion difference(Quaternion q1, Quaternion &q2);

	//print out the data from a Quaternion to the Serial connection (for debugging)
	void printQuat();
	
	//returns the angle that has made the largest movement around an axis (x,y,z)
	int quatAngle();
	
	//return the angle of the Z co-ordinate 
	int quatAngleZ();
	
	//return the angle of the W co-ordinate 
	int quatAngleW();
 
};
 
 #endif //Quaternions_h
  




  
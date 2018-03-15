/**
 * @file   Quaternions.h
 * @author Matt Fielder
 * @date   January - March 2018
 * @brief  Implementation file for quaternion operations.
 */
 
#ifndef Quaternions_cpp //Quaternions_cpp
#define Quaternions_cpp

#include "Quaternions.h"
#include "Arduino.h"
#include <math.h>
#include <MyoBridge.h> 

//variables
float w, x, y, z;
String name;

//constructor
Quaternion::Quaternion() : x(0), y(0), z(0), w(0), name("default"){} 

//pass and store the name of the Quaternion for later use
void Quaternion::passName(String _name)
{
	this->name = _name;
}

//pass data to Quaternion
void Quaternion::passQuaternion(int16_t quat[4]) 
{
	//constrain is used to keep the data within limits as a precaution
	//the data is divided by the scale that the Myo applies to this data by default
	x = constrain((float)quat[0] / (MYOHW_ORIENTATION_SCALE), -.999999, .999999);
	y = constrain((float)quat[1] / (MYOHW_ORIENTATION_SCALE), -.999999, .999999);
	z = constrain((float)quat[2] / (MYOHW_ORIENTATION_SCALE), -.999999, .999999);
	w = constrain((float)quat[3] / (MYOHW_ORIENTATION_SCALE), -.999999, .999999);
};

//Norm-squared
float Quaternion::normSquared()
{ 
	return w*w + x*x + y*y + z*z;
}

//multiply two Quaternions
Quaternion Quaternion::operator* (const Quaternion& q2) 
{ 
	//store the original values for calculations
	//otherwise they will be changed during multiplication and the result will not be correct
	float _x, _y, _z, _w;
		_x = x;
		_y = y;
		_z = z;
		_w = w;
		
	w = _w*q2.w - _x*q2.x - _y*q2.y - _z*q2.z;
	x = _w*q2.x + _x*q2.w + _y*q2.z - _z*q2.y;
	y = _w*q2.y + _y*q2.w + _z*q2.x - _x*q2.z;
	z = _w*q2.z + _z*q2.w + _x*q2.y - _y*q2.x;
	return (*this);
}

//equals: assign the values of one Quaternion to another
//E.G. q1 = q2; q1 will receive the values of q2
Quaternion Quaternion::operator=(const Quaternion& q) 
{ 
	w=q.w; 
	x=q.x; 
	y=q.y; 
	z=q.z; 
	
	//only print out the named quaternions that are needed to send over Serial comms (q1,q2,qd)
	//these will be recognised by the MyoArduino GUI (if using it)
	if (this->name != String("default"))
	{
		this->printQuat();
	}
	return (*this); 
}


//conjugate: returns the conjugate of a Quaternion
Quaternion Quaternion::conjugate() 
{
	this->x = -x;
	this->y = -y;
	this->z = -z;
	this->w = w;
	return (*this);
}

//return a Quaternion that contains the difference between q1 and q2
Quaternion Quaternion::difference(Quaternion q1, Quaternion &q2)
{
	//create objects to be used for calculations
	Quaternion inverse, conj;
	
	//get the squared norm of q1 and store it
	float norm_sq = q1.normSquared();

	//get conjugate of q1
	conj = q1.conjugate();

	//use the conjugate data for finding the inverse of a Quaternion
	inverse = conj;
	//divide each axis of the conjugate data by the squared norm to find the inverse
	inverse.x /= norm_sq;
	inverse.y /= norm_sq;
	inverse.z /= norm_sq;
	inverse.w /= norm_sq;

	//multiply the inverse of q1 by q2 to get the Quaternion difference
	*this = inverse * q2;
	//return the difference quaternion
	return (*this);
}  

//print out the data from a Quaternion to the Serial connection (for debugging)
void Quaternion::printQuat()
{
	Serial.print(name);
	Serial.print(" - x: ");Serial.print(x);
	Serial.print(" ,y: ");Serial.print(y);
	Serial.print(" ,z: ");Serial.print(z);
	Serial.print(" ,w: ");Serial.println(w);
}

//returns the angle that has made the largest movement around an axis (x or y)
int Quaternion::quatAngle()
{
	//store the angle calculations
	int xAngle,yAngle,zAngle;
	//angles can be between -180 and (+)180
	xAngle = x *180;
	yAngle = y *180;
	//zAngle = z *180;

	//Serial.print("xangle:");Serial.print(xAngle);Serial.print(", yangle:");Serial.print(yAngle);Serial.print(", zangle:");Serial.println(zAngle);

	//find the largest angle difference
	//here we try to find the axis the user hoped to use
	int high = max(abs(xAngle), abs(yAngle));
	if (high == abs(xAngle))
	{
		Serial.print("X AXIS MOVEMENT: ");Serial.println(xAngle);
		return xAngle;
	}
	else if (high == abs(yAngle))
	{
		Serial.print("Y AXIS MOVEMENT: ");Serial.println(yAngle);
		return yAngle;
	}

}

//return the angle of the Z co-ordinate 
int Quaternion::quatAngleZ()
{
	//store the angle calculations
	int zAngle;
	//angles can be between -180 and (+)180
	zAngle = z *180;
	return zAngle;
}

//return the angle of the W co-ordinate 
int Quaternion::quatAngleW()
{
	//store the angle calculations
	int wAngle;
	//angles can be between -180 and (+)180
	wAngle = w *180;
	return wAngle;
}


#endif //Quaternions_cpp
  




  
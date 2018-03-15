/**
   @file   Servo_and_piezo.ino
   @author Matt Fielder
   @date   February-March 2018
   @brief  Using Myo poses to control 3 connected LEDs with pose data printed on an LCD.
           Servo is controlled by angles from Quaternions provided by the Myo. 
           
           Speaker/piezo output requires q1 to be set (fist) then wave-in to be performed.
           This ouput command "tone" is located in HandleIMUData() and will be looped with it.
           To stop the speaker/piezo output, perform a different pose (other than wave-in), or reset the
           lastPose variable by performing the "finger tap" pose.

   This file requires the MyoBridge library, which can be found @ https://github.com/vroland/MyoBridge.

*/

#include <MyoBridge.h>
#include <SoftwareSerial.h>
#include <MyoArduino.h>
#include <Quaternions.h>
#include <LiquidCrystal.h>
#include <math.h>
#include <Servo.h>

//initialise LCD library with numbers of the interface pins
const int rs = 10, en = 11, d4 = 7, d5 = 8, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//SoftwareSerial connection to MyoBridge
SoftwareSerial bridgeSerial(2, 3);

//initialise MyoBridge object with software serial connection
MyoBridge bridge(bridgeSerial);

//create objects to handle the quaternion data from the Myo
Quaternion q1, q2, qd, quatUpdate;

//create servo object
Servo servo;

//initialise output object
Output output;

//used to store the last known pose (other than "resting")
int lastPose = 0;

//declare a function to handle pose data
void handlePoseData(MyoPoseData& data)
{
  //convert pose data to MyoPose
  MyoPose pose;
  pose = (MyoPose)data.pose;

  //assign a pose number
  int poseNum = output.poseNumber(bridge.poseToString(pose));
  //LCD top line will be the same for all poses
  const String lcdTopText = "Current pose:";

  //if not "resting" then store the posenumber
  if (poseNum > 1){lastPose = poseNum;}

  //assign poses different actions
  //LED on pin 4 will be controlled by "Wave In"
  //LED on pin 5 will be controlled by "Wave Out"
  //LED on pin 6 will be controlled by "Fingers Spread"
  switch (poseNum)
  {
    case 1: output.outputMessages(lcdTopText, "resting", lcd);
      break;
    case 2: output.outputMessages(lcdTopText, "fist", lcd);
      //set q1 to the current co-ordinates
      q1 = quatUpdate;
      break;
    case 3: output.outputMessages(lcdTopText, "wave in", lcd);
      output.changeLedState(4); //red LED
      break;
    case 4: output.outputMessages(lcdTopText, "wave out", lcd);
      output.changeLedState(5); //blue LED
      //set q2 to the current co-ords
      //then calculate the difference between q1 and q2, then store the resulting quaternion in qd
      q2 = quatUpdate;
      qd.difference(q1,q2);
      //change the servo position based on the calculation from quatAngle()
      output.setServoPosition(servo, qd.quatAngle());
      break;
    case 5: output.outputMessages(lcdTopText, "fingers spread", lcd);
      output.changeLedState(6); //green LED
      break;
    case 6: output.outputMessages(lcdTopText, "finger tap", lcd);
      lastPose = 1;
      break;
  }
}

//function to handle the IMU data (orientation and movement of the Myo)
//this function runs constantly to update the Quaternion position
void handleIMUData(MyoIMUData& dataIMU)
{
  //store the data (this creates an array: quat[4])
  int16_t* quat = {(int16_t*)&dataIMU.orientation};
  //pass the data to quatUpdate
  quatUpdate.passQuaternion(quat);

  //if the last pose was wave in then
  if (lastPose == 3)
  { 
    //update q2 and get qd
    q2 = quatUpdate;
    qd.difference(q1,q2);
    //map the pitch to quatAngle(x or y)
    int pitch = map(qd.quatAngle(),-180,180,50,2000);
    //map the duration of tone to quatAngleZ
    int duration = map(qd.quatAngleZ(),-180,180,20, 200);
    //send the tone to A5 (analog 5 is where the speaker/piezo is connected)
    tone(A5,pitch,duration);
  }
}

void setup()
{
  //initialise both serial connections
  Serial.begin(115200);
  bridgeSerial.begin(115200);

  //initialise the LCD (number of columns and rows)
  lcd.begin(16, 2);

  //wait until MyoBridge has found Myo and is connected.
  output.outputMessages("Detecting Myo", lcd);
  bridge.begin();
  output.outputMessages("Successfully", "connected to Myo", lcd);

  //set the function that handles pose events
  bridge.setPoseEventCallBack(handlePoseData);
  //tell the Myo we want Pose data
  bridge.enablePoseData();
  //make sure Myo is unlocked
  bridge.unlockMyo();
  //you have to perform the sync gesture to receive Pose data (wave out)

  //set the function that handles the IMU data
  bridge.setIMUDataCallBack(handleIMUData);
  //tell the Myo we just want the IMU data
  bridge.setIMUMode(IMU_MODE_SEND_DATA);
  //disable sleep mode, so we get continous data even when not synced
  bridge.disableSleep();
  
  //Set up LEDs as OUTPUTs
  for (int i = 4; i <= 6; i++) //Pins 4,5,6 are connected to LEDs
  {
    pinMode(i, OUTPUT);
    Serial.println(String("Set pin number ") + i + String(" as OUTPUT"));
  }
  //Set up servo and move its position to 0
  servo.attach(9);
  servo.write(0);
  //delay to let the servo move to position before detaching it
  delay(500);
  //detach servo
  servo.detach();
  //this "attach, move, delay, detach" method is described in setServoPosition()

  //setup speaker/piezo on Analog 5 as output
  pinMode(A5,OUTPUT);

  //send name to Quaternion objects
  q1.passName("q1");
  q2.passName("q2");
  qd.passName("qd");
}

void loop()
{
  //update the connection to MyoBridge
  bridge.update();
}

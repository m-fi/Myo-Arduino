/**
 * @file   3LEDs.ino
 * @author Matt Fielder
 * @date   December 2017
 * @brief  Using Myo poses to control 3 connected LEDs.
 *
 * This file requires the MyoBridge library, which can be found @ https://github.com/vroland/MyoBridge.
 * 
 */
 
 
#include <MyoBridge.h>
#include <SoftwareSerial.h>
#include <MyoArduino.h>


//SoftwareSerial connection to MyoBridge
SoftwareSerial bridgeSerial(2,3);

//initialise MyoBridge object with software serial connection
MyoBridge bridge(bridgeSerial);

//declare a function to handle pose data
void handlePoseData(MyoPoseData& data) {
  
  //convert pose data to MyoPose
  MyoPose pose;
  pose = (MyoPose)data.pose;

  //assign a pose number
  Output output;
  int poseNum = output.poseNumber(bridge.poseToString(pose));
  
  //assign poses different actions
  //LED on pin 4 will be controlled by "Fist"
  //LED on pin 5 will be controlled by "Wave Out"
  //LED on pin 6 will be controlled by "Fingers Spread"
  switch(poseNum)
  {
    case 1: Serial.println("Resting");
      break;
    case 2: Serial.println("Fist");
            output.changeLedState(4); //red 
      break;
    case 3: Serial.println("Wave In");
      break;
    case 4: Serial.println("Wave Out");
            output.changeLedState(5); //blue
      break;
    case 5: Serial.println("Fingers Spread");
            output.changeLedState(6); //green
      break;
    case 6: Serial.println("TAPTAP");
      break;

  }
  
}

void setup() {
  //initialise both serial connections
  Serial.begin(115200);
  bridgeSerial.begin(115200);

  //wait until MyoBridge has found Myo and is connected. Make sure Myo is not connected to anything else and not in standby!
  Serial.println("Searching for Myo...");
  bridge.begin();
  Serial.println("connected!");

  //set the function that handles pose events
  bridge.setPoseEventCallBack(handlePoseData);
  //tell the Myo we want Pose data
  bridge.enablePoseData();
  //make sure Myo is unlocked
  bridge.unlockMyo();
  //You have to perform the sync gesture to receive Pose data!

  //Set up LEDs as OUTPUTs
  for (int i=4; i <= 6; i++) //Pins 4,5,6 are connected to LEDs
  {
    pinMode(i, OUTPUT);
    Serial.println(String("Set pin number ") + i + String(" as OUTPUT"));
  }
}

void loop() {
  //update the connection to MyoBridge
  bridge.update();
}

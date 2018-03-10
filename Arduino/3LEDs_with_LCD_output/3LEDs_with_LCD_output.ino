/**
 * @file   3LEDs_with_LCD_output.ino
 * @author Matt Fielder
 * @date   February 2018
 * @brief  Using Myo poses to control 3 connected LEDs with pose data printed on an LCD.
 *
 * This file requires the MyoBridge library, which can be found @ https://github.com/vroland/MyoBridge.
 * 
 */
 
#include <MyoBridge.h>
#include <SoftwareSerial.h>
#include <MyoArduino.h>
#include <LiquidCrystal.h>

//initialise LCD library with numbers of the interface pins
const int rs = 10, en = 11, d4 = 7, d5 = 8, d6 = 12, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//SoftwareSerial connection to MyoBridge
SoftwareSerial bridgeSerial(2,3);

//initialise MyoBridge object with software serial connection
MyoBridge bridge(bridgeSerial);

//declare a function to handle pose data
void handlePoseData(MyoPoseData& data) 
{
  //convert pose data to MyoPose
  MyoPose pose;
  pose = (MyoPose)data.pose;

  //assign a pose number
  Output output;
  int poseNum = output.poseNumber(bridge.poseToString(pose));
  //LCD top line will be the same for all poses
  const String lcdTopText = "Current pose:";

  //assign poses different actions
  //LED on pin 4 will be controlled by "Wave In"
  //LED on pin 5 will be controlled by "Wave Out"
  //LED on pin 6 will be controlled by "Fingers Spread"
  switch(poseNum)
  {
    case 1: output.outputMessages(lcdTopText, "resting", lcd);
      break;
    case 2: output.outputMessages(lcdTopText, "fist", lcd);            
      break;
    case 3: output.outputMessages(lcdTopText, "wave in", lcd);
            output.changeLedState(4); //red LED
      break;
    case 4: output.outputMessages(lcdTopText, "wave out", lcd);
            output.changeLedState(5); //blue LED
      break;
    case 5: output.outputMessages(lcdTopText, "fingers spread", lcd);
            output.changeLedState(6); //green LED
      break;
    case 6: output.outputMessages(lcdTopText, "finger tap", lcd);
      break;
  }
}

void setup() 
{
  //initialise both serial connections
  Serial.begin(115200);
  bridgeSerial.begin(115200);

  //initialise the LCD (number of columns and rows)
  lcd.begin(16,2);

  //initialise output object
  Output output;
  
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

  //Set up LEDs as OUTPUTs
  for (int i=4; i <= 6; i++) //Pins 4,5,6 are connected to LEDs
  {
    pinMode(i, OUTPUT);
    Serial.println(String("Set pin number ") + i + String(" as OUTPUT"));
  }
}

void loop() 
{
  //update the connection to MyoBridge
  bridge.update();
}

# Myo-Arduino GUI
The GUI is coded with QT5.8.
The GUI purpose is to recieve serial communication sent from the Arduino, the parse the data to find the Myo positioning data (in quaternions). 
The quaternions are then displayed as 3d representations in a window created using OpenGL 3.2 and AntTweakBar.
## Layout
<p align="center"><img src="./docs/MyoArduino_GUI.png"></p>

## Requirements

Compiling requires:
* [Qt 5.8+](https://www.qt.io/) (created with 5.8)
* AntTweakBar (included in [include/](MyoArduino_GUI/include/))

Required to run:
* OpenGL 3.2 capable GFX card/drivers (released in 2009 so most modern pcs should be able to run this)

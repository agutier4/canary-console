# me205-lidar
Hosting for Team Lidar software for ME205.

## CMake build instructions
- make build directory (mkdir build)
- enter build directory (cd build)
- run cmake from build directory and point to CMakeLists.txt (cmake ..)
- run make in build directory (make or make VERBOSE=1 if you want debugging info)
- __NOTE:__ _hardware executables will only be built on pi (UNIX AND !APPLE)_

## Navigation
- *simulateData.cpp:* use virtual lidar to simulate data and broadcast to LCM
- *viewer:* viewer for lidar points, will accept and all xyzLdr data streamed over LCM
- *lidarTest.cpp:* interface with rplidar and stream data to LCM
- *lidarNode.cpp:* lidar node setup to control laser range finder with input to LIDAR_CMND channel on LCM streams to DATA_STREAM
- *motorNode.cpp:* node to control motors and read sensor data with input from MOTOR_CMND channel on LCM
- *canarySerial.cpp:* node that takes in serial data over radio and converts it to LCM

## Simulating Data with Virtual lidar
  - stream to DATA_STREAM LCM channel with simulateData
  - run simulateData with intended scan type parameter (./simulateData [scanType])
  - Supported scan types:
    - 1: Cube scan
    - 2: Circle Scan  
    - 3: Sine Scan

## creating LCM data type
  - Create a file "${dataType}_t.lcm"
  - populate by starting first line with "package ${dataType};"
  - create struct with the desired variables for the data type
  - in the same directory as ${dataType}_t.lcm, run "lcm-gen -x ${dataType}_t.lcm"
  - Folder with the name "${dataType}" should be created with file "${dataType}_t.hpp" in it
    - DO NOT EDIT THAT FILE
  - Make sure to #include/${dataType}.hpp whenever that variable is used

## WiringPi GPIO ISR instructions
  - To set a pin to a given ISR mode use the following command
    - gpio edge [pinNumber] [ISR_Mode]
    - ISR modes:
      - rising
      - falling
      - both
      - none
  - Remember to rest ISR pin to "none" when finished


## The use of LCM
### Lightweight Communication and Marshalling (LCM) Types
  - LCM allows data types to be transmitted.
  - This allows the ease of interprocess/internodal communication, which allows each node to use a subset of variables from another node.

## Programs and Their Functionality
### lidar.cpp
  - Uses a rp-LIDAR A2M6 API to communicate and use LIDAR
  - Program to initiate the LiDAR.
  - Functions such as starting the motor of the LIDAR and starting the scan are all in this program


### lidarScan.cpp
  - Setting up the LIDAR subscriber which listens to the lidar commands from lidar.cpp

### simulateData.cpp
  - A test program written to test the viewer built in viewer.cpp
  - Produces 3D shapes such as a cube, helix and sine wave

### transiever.py
  - Program written for the communication over radio between the Rapsberry Pi 3+ and the users laptop
  - Allows live feed of data to the user

### viewer.cpp
  - Subscriber set for subscribing to the viewerWidget node to handle messages from that program

### viewerWidget.cpp
  - Using the Qt library to set up a frame on which the collected data from the LiDAR can be drawn on
  - Capable of sending commands to run the motor, reverse direction and stop it throughout the LIDAR run

### virtualLidar.cpp
  - Similar to simulateData.cpp, where sample scans and shapes are made.


#include "lidar.h"
#include <iostream>
#include <chrono>

int main(int argc, char ** argv){
  Lidar lidar;
  
  lidar.connect();
  lidar.startMotor();
  lidar.lcm.subscribe("LIDAR_CMND", &Lidar::handleMessage,&lidar);

  for(size_t scanCount =0; scanCount <= 100; scanCount++){
    std::cout<< "Starting Scan" << scanCount <<std::endl;
    lidar.scan(8120,"DATA_STREAM");
    lidar.lcm.handleTimeout(10);
  }
  lidar.stopMotor();
  lidar.disconnect();
}

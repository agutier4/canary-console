#include "lidar.h"
#include <iostream>
#include <chrono>

int main(int argc, char ** argv){
  Lidar lidar;
  
  lidar.connect();
  lidar.startMotor();
  lidar.lcm.subscribe("LIDAR_CMND", &Lidar::handleMessage,&lidar);

  for(size_t scanCount =0; scanCount <= 100; scanCount++){
    //auto start = std::chrono::high_resolution_clock::now();
    std::cout<< "Starting Scan" << scanCount <<std::endl;
    lidar.scan(8120,"DATA_STREAM");
    //auto finish = std::chrono::high_resolution_clock::now();
    //std::chrono::duration<double> elapsed = finish-start;
    //std::cout << "Elapsed time: " <<elapsed.count() <<" s\n";
    lidar.lcm.handleTimeout(10);
  }
  lidar.stopMotor();
  lidar.disconnect();

  /*
  //test lcm commands recived over radio
  lidar.lcm.subscribe("LIDAR_CMND", &Lidar::handleMessage,&lidar);
  while(true){
    lidar.lcm.handle();
  }
  return 0;
  */
}

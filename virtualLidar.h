#ifndef VIRTUAL_LIDAR_H
#define VIRTUAL_LIDAR_H

#include <lcm/lcm-cpp.hpp>
#include "xyzLdr/xyzLidar_t.hpp"
#include <math.h>
#include <iostream>
#include <unistd.h>

class VirtualLidar{
  public:
    VirtualLidar() : lcm( "udpm://239.255.76.67:7667?ttl=1" ) {};
    virtual ~VirtualLidar(){};
    void scanCube();
    void scanCircle();
    void scanSin();

    lcm::LCM lcm;
};

#endif /* VIRTUAL_LIDAR_H */

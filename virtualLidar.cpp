#include "virtualLidar.h"

using namespace std;

void
VirtualLidar::
scanCube(){
  xyzLdr::xyzLidar_t lidarPoint;
  lidarPoint.quality = 0;
  int t = 0;
  for (double i = 0; i <= 2; i+=.25) {
  	for (double j = 0; j <= 2; j+=.25) {
  		for (double k = 0; k <= 2; k+=.25) {
  			t++;
  			lidarPoint.timestamp = t;
  			lidarPoint.x = i;
  			lidarPoint.y = j;
  			lidarPoint.z = k;
  			lcm.publish("LIDAR_STREAM", &lidarPoint);
  			usleep(5000);
  		}
  	}
  }
}

void
VirtualLidar::
scanCircle(){
  xyzLdr::xyzLidar_t lidarPoint;
  int t = 0;
  int r = 2.0;
  lidarPoint.quality = 0;
  for (double j= 0; j <= 2.0; j+=.25) {
    for (int i = 0; i <= 360; i++) {
      t++;
      lidarPoint.timestamp = t;
      lidarPoint.x = r*cos(i*M_PI/180);
      lidarPoint.y = r*sin(i*M_PI/180);
      lidarPoint.z = j;
      lcm.publish("LIDAR_STREAM", &lidarPoint);
      usleep(5000);
    }
  }

}

void
VirtualLidar::
scanSin(){
  xyzLdr::xyzLidar_t lidarPoint;
  int t = 0;
	for (double i = 0; i <= 1.0; i+=.1) {
		for (double k = 0; k <= 5.0; k+=.1) {
			t++;
			lidarPoint.timestamp = t;
			lidarPoint.x = k;
			lidarPoint.y = i;
			lidarPoint.z = sin(k);
			lcm.publish("DATA_STREAM", &lidarPoint);
			usleep(10000);
		}
	}
}

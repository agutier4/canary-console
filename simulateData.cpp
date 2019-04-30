#include <iostream>
#include <lcm/lcm-cpp.hpp>
#include "xyzLdr/xyzLidar_t.hpp"
#include <math.h>
#include <unistd.h>
#include "virtualLidar.h"

int main(int argc, char ** argv){
	if(argc>=2){
		VirtualLidar virtualLidar;
		if(!virtualLidar.lcm.good()){
			std::cout << "ERROR: LCM initialization failure" <<std::endl;
			return 1;
		}

		switch (atoi(argv[1])) {
			case 1:
				std::cout<< "Begin cube scan" <<std::endl;
				virtualLidar.scanCube();
				break;
			case 2:
				std::cout<< "Begin circle scan" <<std::endl;
				virtualLidar.scanCircle();
				break;
			case 3:
				std::cout<< "Begin sine scan" <<std::endl;
				virtualLidar.scanSin();
				break;
		}

		return 0;
	}
	std::cout<<"ERROR: Please specify scan type"<<std::endl;
	std::cout<<"i.e. \"./simulateData 1\""<<std::endl;
	std::cout<<"Supported scan types:"<<std::endl;
	std::cout<<"1: Cube"<<std::endl;
	std::cout<<"2: Circle"<<std::endl;
	std::cout<<"3: Sine"<<std::endl;

	return 1;
}

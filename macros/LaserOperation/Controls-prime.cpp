
#include <zaber/motion/ascii.h>
#include <iostream>
#include <fstream>
#include <cstring>
#include <string>
#include <vector> 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

using namespace std;
using namespace zaber::motion;
using namespace zaber::motion::ascii;

void readFile (char *myFile, vector<int>& x, vector<int>& y) {
	int a,b;
	std::ifstream file(myFile);
	int i = 0;

	while (!file.eof()) {

		file >> a >> b;
		x.push_back(a);
		y.push_back(b);
		cout << x[i] << "  " << y[i] << endl;
		i++;
	}

}

//create the shell command line text
string createCommand (char* name, int xLocation, int yLocation) {

	std::string daq = "daq commands ";		
	daq = daq + name + "save file" + "_" + std::to_string(xLocation) + "_" + std::to_string(yLocation);
	cout << daq << endl;
	std::vector<char> cstr(daq.c_str(), daq.c_str() + daq.size() + 1);
	std::string command (cstr.begin(), cstr.end() );
	return command;
		
}

void scanning(int X, int Y, vector<Axis> axis, string commandString) {

	int deltaX = 1;
	int deltaY = 1;

	axis[0].moveAbsolute(X , Units::LENGTH_MILLIMETRES); 
	axis[1].moveAbsolute(Y , Units::LENGTH_MILLIMETRES); 
		

	for(int n = 0; n < 20 ; n++) {
		for (int m = 0; m < 20; m++) {
			axis[1].moveRelative(deltaY, Units::LENGTH_MILLIMETRES);			
				
			
			const char* command = commandString.c_str();
			system(command);					//run DAQ test
			delete []command;	
			}

		axis[0].moveRelative(deltaX, Units::LENGTH_MILLIMETRES);	
		axis[1].moveAbsolute(Y, Units::LENGTH_MILLIMETRES);
		}
}



int main(int argc, char *argv[]) {

//Open the configuration file that will have the upper left corner for each scanning
	if (argc != 2) {
		return 0;
	}

	std::vector<int> x;
	std::vector<int> y;
	readFile(argv[1],x,y);
	std::string daqCommand;



	Connection connection = Connection::openSerialPort("/dev/ttyUSB0");

// connection is previously opened Connection
	std::vector<Device> deviceList = connection.detectDevices();

	std::cout << "Found " << deviceList.size() << " devices" << std::endl;
	std::vector<Axis> axes;

	int j = 0;

	for (auto& device: deviceList) {
		std::cout << "Homing all axes of device with address " << device.getDeviceAddress() << "." << std::endl;

	// Used to set all the indivdual motors
		axes.push_back(deviceList[j].getAxis(1));
		axes[j].home();
		j++;
	}



	axes[2].moveAbsolute(10, Units::LENGTH_MILLIMETRES); //set z-axis postion 

	for (int cornerX = 0; cornerX < x.size(); cornerX++) {
		for (int cornerY = 0; cornerY < y.size(); cornerY++) {

			daqCommand = createCommand(argv[2], x[cornerX] + cornerX, y[cornerY] + cornerY);
			scanning(x[cornerX], y[cornerY], axes,daqCommand);
			
		}
	}

	return 0;
}



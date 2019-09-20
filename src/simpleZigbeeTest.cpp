//============================================================================
// Name        : simpleZigbeeTest.cpp
// Author      : Hemant Kapoor
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include <memory>
#include <thread>
#include <vector>
#include <future>
#include"simpleSerial/comms/Comms.h"
#include "simpleZigbee/manager/Manager.h"


char GetUserOption()
{
    char option;
    std::cin.get(option);
    return option;
}

int main() {
	std::string serialPath =  R"(/dev/ttyACM0)";
	auto sp = std::make_shared<SimpleSerialName::Comms>(serialPath);
	if(sp->startComms() == false)
	{
		std::cout<<__PRETTY_FUNCTION__<<" : Cannot access serial port " <<serialPath <<std::endl;
		return -1;
	}

	auto zibMan = std::make_unique<SimpleZigbeeName::ZigbeeManager>(sp);

	//Lets try to initialize sigbee
	if(zibMan->initialise() == false)
	{
		std::cout<<__PRETTY_FUNCTION__<<" : Initialisation of zigbee module failed \r\n";
		return -2;
	}

	std::cout<<__PRETTY_FUNCTION__<<" : Serial port Running... Press q to quit\r\n";
	char option;
	auto future = std::async(std::launch::async, GetUserOption);

	while(option != 'q')
	{
		if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			option = future.get();
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	//We are here so do graceful exit
	std::cout<<__PRETTY_FUNCTION__<<" : Closing Application\r\n";

	return 0;
}

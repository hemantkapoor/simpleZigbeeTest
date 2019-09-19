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
#include"simpleSerial/callback/BaseCallback.h"
#include"simpleSerial/utility/Utility.h"

#include "simpleZigbee/zigbeeManager/ZigbeeManager.h"


char GetUserOption()
{
    char option;
    std::cin.get(option);
    return option;
}

class testSerialCallback : public SimpleSerialName::BaseCallback
{
public:
	testSerialCallback() = default;
	void callback(std::vector<uint8_t>& data) override
	{
		std::cout<<__PRETTY_FUNCTION__<< " : " << SimpleSerialName::Utility::hexStr(data.data(),data.size())<<std::endl;
	}
};

int main() {
	std::cout << "!!!Hello World!!!" << std::endl; // prints !!!Hello World!!!
	std::string serialPath =  R"(/dev/pts/19)";
	auto sp = std::make_shared<SimpleSerialName::Comms>(serialPath);
	if(sp->startComms() == false)
	{
		std::cout<<__PRETTY_FUNCTION__<<" : Cannot access serial port\r\n";
		return -1;
	}

	auto zibMan = std::make_unique<SimpleZigbeeName::ZigbeeManager>(sp);

	//Lets try to initialize sigbee
	if(zibMan->initialise() == false)
	{
		std::cout<<__PRETTY_FUNCTION__<<" : Initialisation of zigbee module failed \r\n";
		return -2;
	}

	//Lets add a callback
	auto myCallback = std::make_shared<testSerialCallback>();
	sp->addCallback(myCallback);
	//If here then we better spawn a thread
	std::cout<<__PRETTY_FUNCTION__<<" : Serial port Running... Press q to quit\r\n";
	char option;

	std::vector<uint8_t> transmitData{'H','E','L','L','O','\r','\n'};

	auto future = std::async(std::launch::async, GetUserOption);

	while(option != 'q')
	{
		if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
		{
			option = future.get();
		}
		//For test purpose lets write
		if(sp->transmitData(transmitData) == false)
		{
			std::cout<<__PRETTY_FUNCTION__<<" : Transmit Failed\r\n";
		}
		std::this_thread::sleep_for(std::chrono::milliseconds(1000));
	}
	//We are here so do graceful exit
	std::cout<<__PRETTY_FUNCTION__<<" : Closing Application\r\n";

	return 0;
}

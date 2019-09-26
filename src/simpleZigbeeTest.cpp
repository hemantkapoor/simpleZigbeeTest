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
#include <experimental/filesystem>
#include <string>
#include "simpleDebug/SimpleDebug.h"
#include"simpleSerial/comms/Comms.h"
#include "simpleZigbee/manager/Manager.h"

namespace fs = std::experimental::filesystem;



char GetUserOption()
{
    char option;
    std::cin.get(option);
    return option;
}

int main()
{
	//Lets get current directory
	auto pwd = fs::current_path();
	auto newDir = pwd / "debugLog";
	fs::create_directory(newDir);
	auto debugFile = newDir / "debugLog.txt";

	auto m_debug = SimpleDebugName::SimpleDebug::instance();
	m_debug->setDebugFile(debugFile);
	m_debug->setDebugMaskConsole(SimpleDebugName::LOG);

	m_debug->log(SimpleDebugName::NONE, std::string(__PRETTY_FUNCTION__) + " : Starting Simple Zigbee Application\r\n");

	std::string serialPath =  R"(/dev/ttyACM0)";
	auto sp = std::make_unique<SimpleSerialName::Comms>(serialPath);
	if(sp->startComms() == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Cannot access serial port " + serialPath);
		return -1;
	}
	auto zibMan = std::make_unique<SimpleZigbeeName::ZigbeeManager>(std::move(sp));

	m_debug->log(SimpleDebugName::NONE, std::string(__PRETTY_FUNCTION__) + " : Press q to quit\r\n");
	//Lets try to initialise Zigbee
	if(zibMan->initialise() == false)
	{
		m_debug->log(SimpleDebugName::CRITICAL_ERROR, std::string(__PRETTY_FUNCTION__) + " : Initialisation of zigbee module failed \r\n");
		return -2;
	}
	char option;
	{
		auto future = std::async(std::launch::async, GetUserOption);

		while(option != 'q')
		{
			if (future.wait_for(std::chrono::seconds(0)) == std::future_status::ready)
			{
				option = future.get();
			}
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
		}
	}
	//We are here so do graceful exit
	m_debug->log(SimpleDebugName::LOG, std::string(__PRETTY_FUNCTION__) + " : Closing Application\r\n");
	return 0;
}

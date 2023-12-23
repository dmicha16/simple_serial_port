#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>

class SimpleSerial
{

private:
	HANDLE io_handler_;
	COMSTAT status_;
	DWORD errors_;

	std::string syntax_name_;
	char front_delimiter_;
	char end_delimiter_;

	void CustomSyntax(std::string syntax_type);	

public:
	SimpleSerial(char* com_port, DWORD COM_BAUD_RATE);
	void init(char* com_port, DWORD COM_BAUD_RATE);

	std::string ReadSerialPort(int reply_wait_time, std::string syntax_type);	
	bool WriteSerialPort(char *data_sent);
	bool CloseSerialPort();
	~SimpleSerial();
	bool connected_;
};


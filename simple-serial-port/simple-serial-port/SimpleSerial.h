#pragma once

#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <iostream>

class SimpleSerial
{

private:
	HANDLE io_handler_;
	bool connected_;
	COMSTAT status_;
	DWORD errors_;	

public:
	SimpleSerial(char* com_port, DWORD COM_BAUD_RATE);

	char *ReadSerialPort();
	bool WriteSerialPort(char *data_sent);
	bool CloseSerialPort();
	~SimpleSerial();
};


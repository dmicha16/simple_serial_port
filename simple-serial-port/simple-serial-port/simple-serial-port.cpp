// simple-serial-port.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SimpleSerial.h"

using namespace std;

int main()
{
	char com_port[] = "\\\\.\\COM8";
	DWORD COM_BAUD_RATE = CBR_9600;
	SimpleSerial Serial(com_port, COM_BAUD_RATE);

	printf("What would you like to send?\n");
	string read_in;
	cin >> read_in;

	char *to_send = &read_in[0];
	bool work = Serial.WriteSerialPort(to_send);

	int reply_wait_time = 7;
	string syntax_type = "json";
	if (work)
	{	
		string incoming = Serial.ReadSerialPort(7, syntax_type);
		cout << incoming;
		while (1) {}
	}	
    return 0;
}


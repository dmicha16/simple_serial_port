#include "stdafx.h"
#include "SimpleSerial.h"

SimpleSerial::SimpleSerial(char* com_port, DWORD COM_BAUD_RATE)
{
	connected_ = false;

	io_handler_ = CreateFileA(static_cast<LPCSTR>(com_port),
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

	if (io_handler_ == INVALID_HANDLE_VALUE) {

		if (GetLastError() == ERROR_FILE_NOT_FOUND)
			printf("ERROR: Handle was not attached. Reason: %s not available\n", com_port);
	}
	else {

		DCB dcbSerialParams = { 0 };

		if (!GetCommState(io_handler_, &dcbSerialParams)) {

			printf("failed to get current serial parameters");
		}

		else {
			dcbSerialParams.BaudRate = COM_BAUD_RATE;
			dcbSerialParams.ByteSize = 8;
			dcbSerialParams.StopBits = ONESTOPBIT;
			dcbSerialParams.Parity = NOPARITY;
			dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

			if (!SetCommState(io_handler_, &dcbSerialParams))
				printf("ALERT: could not set Serial port parameters\n");
			else {
				connected_ = true;
				PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);				
			}
		}
	}
}

string SimpleSerial::ReadSerialPort(int reply_wait_time, string syntax_type)
{
	DWORD bytes_read;
	char inc_msg[1];	
	string complete_inc_msg;
	bool began = false;

	unsigned long start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);	

	while ((time(nullptr) - start_time) < reply_wait_time)
	{
		if (status_.cbInQue > 0) {
			
			if (ReadFile(io_handler_, inc_msg, 1, &bytes_read, NULL)) {

				if (syntax_type == "json") {
					if (inc_msg[0] == '{' || began) {
						began = true;

						if (inc_msg[0] == '}')
							return complete_inc_msg;

						if (inc_msg[0] != '{') {
							complete_inc_msg.append(inc_msg, 1);
						}
					}
				}
				else if (syntax_type == "greater_less_than") {
					if (inc_msg[0] == '<' || began) {

						began = true;

						if (inc_msg[0] == '>')
							return complete_inc_msg;

						if (inc_msg[0] != '<') {
							complete_inc_msg.append(inc_msg, 1);
						}
					}
				}
				else {					
					return "No expected delimiter.";
				}
				
			}
			else
				return "Warning: Failed to receive data.\n";
		}
	}
	return complete_inc_msg;		
}

bool SimpleSerial::WriteSerialPort(char *data_sent)
{
	DWORD bytes_sent;

	unsigned int data_sent_length = strlen(data_sent);

	if (!WriteFile(io_handler_, (void*)data_sent, data_sent_length, &bytes_sent, NULL)) {
		ClearCommError(io_handler_, &errors_, &status_);
		return false;
	}
	else
		return true;
}

bool SimpleSerial::CloseSerialPort()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);
		return true;
	}	
	else
		return false;
}

SimpleSerial::~SimpleSerial()
{
	if (connected_) {
		connected_ = false;
		CloseHandle(io_handler_);		
	}
}

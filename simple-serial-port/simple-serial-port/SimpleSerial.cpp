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
		else
			printf("ERROR!!!");
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

string SimpleSerial::ReadSerialPort()
{
	DWORD bytes_read;
	char inc_msg[1];
	char *ptr = &inc_msg[0];
	string complete_inc_msg;

	unsigned long start_time = time(nullptr);

	ClearCommError(io_handler_, &errors_, &status_);	

	while ((time(nullptr) - start_time) < 7)
	{
		if (status_.cbInQue > 0) {
			//data_received_length = status_.cbInQue;
			
			//cout << data_received_length << "\n";

			/*if (data_received_length > 255) {
			char data_received_long[511] = { 0 };

			if (ReadFile(io_handler_, data_received_long, data_received_length, &bytes_read, NULL)) {
			data_received_str = data_received_long;

			return data_received_str;
			}
			else
			return "Warning: Too large incoming size. Avoid using large char arrays due to buffer overflows.\n";
			}*/

			if (ReadFile(io_handler_, ptr, 1, &bytes_read, NULL)) {

				complete_inc_msg.append(inc_msg, 1);
				//cout << complete_inc_msg;
			}
			else
				return "Warning: Failed to receive data.\n";
		}
	}
	return complete_inc_msg;
	return "all done!\n";
	//return "Warning: Failed to receive data.\n";
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

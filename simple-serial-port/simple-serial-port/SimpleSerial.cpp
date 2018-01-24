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

char *SimpleSerial::ReadSerialPort()
{
	DWORD bytes_read;
	unsigned int data_received_length = 0;
	char data_received[] = { 0 };

	ClearCommError(io_handler_, &errors_, &status_);

	if (status_.cbInQue > 0)
		data_received_length = status_.cbInQue;
	else
		return 0;

	if (ReadFile(io_handler_, data_received, data_received_length, &bytes_read, NULL))
		return data_received;
	else
		return 0;
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

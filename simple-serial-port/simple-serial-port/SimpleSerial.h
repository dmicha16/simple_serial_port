#pragma once

#include <Windows.h>
#include <string>


class SimpleSerial
{

private:
    HANDLE io_handler_;
    COMSTAT status_;
    DWORD errors_;

    std::string syntax_name_;
    char front_delimiter_;
    char end_delimiter_;
    bool connected_ = false;

    void CustomSyntax(std::string syntax_type);	

public:
    SimpleSerial(const std::string& com_port, DWORD COM_BAUD_RATE);
    ~SimpleSerial();

    void init(const std::string& com_port, DWORD COM_BAUD_RATE);
    std::string ReadSerialPort(int reply_wait_time, const std::string& syntax_type);
    bool WriteSerialPort(const std::string& data_sent);
    bool CloseSerialPort();
    bool IsConnected();
};


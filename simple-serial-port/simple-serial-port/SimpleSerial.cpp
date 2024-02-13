#include "SimpleSerial.h"

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <chrono>
#include <thread>
#include <time.h>
#include <fstream>

SimpleSerial::SimpleSerial(const std::string &com_port, DWORD COM_BAUD_RATE)
{
    this->init(com_port, COM_BAUD_RATE);
}

SimpleSerial::~SimpleSerial()
{
    if (connected_)
    {
        connected_ = false;
        CloseHandle(io_handler_);
    }
}

void SimpleSerial::init(const std::string &com_port, DWORD COM_BAUD_RATE)
{
    if (connected_ == true)
    {
        std::cout << "Warning: could not initialize COM port already in use\n";
        return;
    }

    io_handler_ = CreateFileA(com_port.c_str(),
                                GENERIC_READ | GENERIC_WRITE,
                                0,
                                nullptr,
                                OPEN_EXISTING,
                                FILE_ATTRIBUTE_NORMAL,
                                nullptr);

    if (io_handler_ == INVALID_HANDLE_VALUE)
    {
        if (GetLastError() == ERROR_FILE_NOT_FOUND)
        {
            std::cout << "Warning: Handle was not attached. Reason: " << com_port << " not available\n";
        }
    }
    else
    {
        DCB dcbSerialParams = {0};

        if (!GetCommState(io_handler_, &dcbSerialParams))
        {
            std::cout << "Warning: Failed to get current serial params\n";
        }
        else
        {
            dcbSerialParams.BaudRate = COM_BAUD_RATE;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

            if (!SetCommState(io_handler_, &dcbSerialParams))
            {
                std::cout << "Warning: could not set serial port params\n";
            }
            else
            {
                connected_ = true;
                PurgeComm(io_handler_, PURGE_RXCLEAR | PURGE_TXCLEAR);
            }
        }
    }
}

void SimpleSerial::CustomSyntax(const std::string& syntax_type)
{
    std::ifstream syntaxfile_exist("syntax_config.txt");

    if (!syntaxfile_exist)
    {
        std::ofstream syntaxfile;
        syntaxfile.open("syntax_config.txt");

        if (syntaxfile)
        {
            syntaxfile << "json { }\n";
            syntaxfile << "greater_less_than < >\n";
            syntaxfile.close();
        }
    }

    syntaxfile_exist.close();

    std::ifstream syntaxfile_in;
    syntaxfile_in.open("syntax_config.txt");

    std::string line;
    bool found = false;

    if (syntaxfile_in.is_open())
    {
        while (syntaxfile_in)
        {
            syntaxfile_in >> syntax_name_ >> front_delimiter_ >> end_delimiter_;
            getline(syntaxfile_in, line);

            if (syntax_name_ == syntax_type)
            {
                found = true;
                break;
            }
        }

        syntaxfile_in.close();

        if (!found)
        {
            syntax_name_ = "";
            front_delimiter_ = ' ';
            end_delimiter_ = ' ';
            std::cout << "Warning: Could not find delimiters, may cause problems!\n";
        }
    }
    else
    {
        std::cout << "Warning: No file open\n";
    }
}

std::string SimpleSerial::ReadSerialPort(int reply_wait_time, const std::string& syntax_type)
{
    DWORD bytes_read;
    char inc_msg[1];
    std::string complete_inc_msg;
    bool began = false;

    CustomSyntax(syntax_type);

    time_t start_time = time(nullptr);

    ClearCommError(io_handler_, &errors_, &status_);

    while ((time(nullptr) - start_time) < reply_wait_time)
    {
        if (status_.cbInQue > 0)
        {
            if (ReadFile(io_handler_, inc_msg, 1, &bytes_read, nullptr))
            {
                if (inc_msg[0] == front_delimiter_ || began)
                {
                    began = true;

                    if (inc_msg[0] == end_delimiter_)
                    {
                        return complete_inc_msg;
                    }

                    if (inc_msg[0] != front_delimiter_)
                    {
                        complete_inc_msg.append(inc_msg, 1);
                    }
                }
            }
            else
            {
                return "Warning: Failed to receive data.\n";
            }
        }
    }
    return complete_inc_msg;
}

bool SimpleSerial::WriteSerialPort(const std::string &data_sent)
{
    DWORD bytes_sent;

    unsigned int data_sent_length = static_cast<unsigned int>(data_sent.length());

    if (!WriteFile(io_handler_, data_sent.c_str(), data_sent_length, &bytes_sent, nullptr))
    {
        ClearCommError(io_handler_, &errors_, &status_);
        return false;
    }
    else
    {
        return true;
    }
}

bool SimpleSerial::CloseSerialPort()
{
    if (connected_)
    {
        connected_ = false;
        CloseHandle(io_handler_);
        return true;
    }
    else
    {
        return false;
    }
}

bool SimpleSerial::IsConnected() const
{
    return this->connected_;
}

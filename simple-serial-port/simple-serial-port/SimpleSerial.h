#pragma once

#include <Windows.h>
#include <string>

/**
 * @brief A class to handle serial port communication with a Arduino (or other serial device)
 */
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

    void CustomSyntax(const std::string& syntax_type);	

public:
    /**
     * @brief Construct a new Simple Serial object
     * 
     * @param com_port The serial port to be used for the connection;
     * @param COM_BAUD_RATE The baud rate to be used for the connection;
     */
    SimpleSerial(const std::string& com_port, DWORD COM_BAUD_RATE);

    /**
     * @brief Destroy the Simple Serial object and close the serial port connection
     */
    ~SimpleSerial();

    /**
     * @brief Initialize the serial port connection
     * 
     * @param com_port The serial port to be used for the connection;
     * @param COM_BAUD_RATE The baud rate to be used for the connection;
     */
    void init(const std::string& com_port, DWORD COM_BAUD_RATE);

    /**
     * @brief Read from the serial port
     * 
     * @param reply_wait_time Time to wait for a reply from the serial port;
     * @param syntax_type Syntax type to be used to read from the serial port;
     * @return std::string - the data read from the serial port;
     */
    std::string ReadSerialPort(int reply_wait_time, const std::string& syntax_type);

    /**
     * @brief Write to the serial port
     * 
     * @param data_sent data to be sent to the serial port;
     * @return true - if the data was sent to the serial port;
     * @return false = if the data was not sent to the serial port;
     */
    bool WriteSerialPort(const std::string& data_sent);

    /**
     * @brief Close the serial port connection
     */
    bool CloseSerialPort();

    /**
     * @brief Check if we are connected to the serial port
     * 
     * @return true - if we are connected to the serial port;
     * @return false - if we are not connected to the serial port;
     */
    bool IsConnected() const;
};


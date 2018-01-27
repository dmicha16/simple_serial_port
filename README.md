# Simple serial communication between a Windows C++ application and an Arduino

During a university project which used an Ardunio and a C++ program, we had some issues of establishing a quick and easy serial port communication. This project was then created after the semester to avoid further problems like this.

This lib allows a simple serial port communication integration into any C++ application that intends to talk to an Ardunio. Reading from Serial port uses common delimiters, such as JSON, which are appended at the beginning and end of each string sent from the Ardunio. Examples are provided further below.

# Features:

  - Write to Serial port
  - Read from Serial port
  - Close connection when needed


### You can also:
  - Set the waiting time for the reply from the Arduino
  - Use common delimiters
  - Add custom delimiters via a local config file

### Initialization the program
The program allows you to set the com port and the baud rate through the constructor. To use the baud rate, due to Windows API, a CBR_ must be added, as shown below. The constructor also sets *connected_* to *true* if the connection has been established successfully.
``` c++
#include "SimpleSerial.h"

char com_port[] = "\\\\.\\COM8";
DWORD COM_BAUD_RATE = CBR_9600;
SimpleSerial Serial(com_port, COM_BAUD_RATE);

if(connected_) {
    //do whatever
}
```

### Reading from Serial port
To read from the Serial port you must call the *ReadSerialPort* function. It takes two parameters, first an integer which represents, in seconds, the time the function should wait for a reply from the Ardunio, and the name of the syntax type, the delimiters expected from the Arduino, either the two default ones (*JSON* or *greater_less_than*), or the ones you create in the config file. That is described further below.

``` c++
int reply_wait_time = 7;
string syntax_type = "mine";

string incoming = Serial.ReadSerialPort(reply_wait_time, syntax_type);
```
The received string is then returned without the delimiters. If no delimiters are found in the config file, or the reading has failed for any reason, the function will return an appropriate warning message.


### Writing to Serial port
Call this function and input your char* that you would like to write. Returns true if writing was successful.

``` c++
bool is_sent = Serial.WriteSerialPort(to_send);

if (is_sent) {
    //do whatever
}
```

It is also easy to send strings, however you will have to create a pointer to your string. Example code:

```c++
printf("What would you like to send?\n");
string read_in;
cin >> read_in;

char *to_send = &read_in[0];
bool work = Serial.WriteSerialPort(to_send);
	
if (is_sent) {
    //do whatever
}
```

### Closing Serial port
Simple. Call this function when you would like to close the Serial port. Returns *true* if successful.
``` c++
bool closed_  = Serial.CloseSerialPort();

if(closed_) {
    //do whatever
}
```
### Creating custom syntax
There is an option to create custom delimtier syntax in a local config file. The config file is generated locally when the program is run for the first time. To create your own custom syntax you can either add it manually into the program,
``` c++
if (syntaxfile) {
		syntaxfile << "json { }\n";
		syntaxfile << "greater_less_than < >\n";
		syntaxfile << "example [ ]\n"
		syntaxfile.close();
	}	
```
or insert it directly into the sytax_config.txt file as a new line, sperated with spaces:

``` c++
json { }
greater_less_than < >
example [ ]
```

### Arduino side

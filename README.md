# Simple serial communication between a Windows C++ application and an Arduino

During a university project which used an Arduino and a C++ program, we had some issues establishing a quick and easy serial port communication system. This project was then created, after the semester, to create an easy to use lightweight serial communication library.

This library allows a simple-serial-port communication integration into any Windows C++ application that intends to communicate with any Ardunio. Reading from the Serial port uses common delimiters, such as JSONs, which are then concatonated at the beginning and at end of each string sent from the Ardunio. Examples are provided further below.

# Features:

  - Write to Serial port
  - Read from Serial port
  - Close connection when needed


### You can also:
  - Set the waiting time for the reply from the Arduino
  - Use common delimiters
  - Add custom delimiters via a local config file

### Initialization the program
The program allows you to set the specific COM PORT and the BAUD RATE through the constructor. To properly use the BAUD RATE, due to Windows a API, a CBR_ notation must be added, as shown below. The constructor also sets *connected_* to *true* if the connection has been established successfully.
``` c++
#include "SimpleSerial.h"

char com_port[] = "\\\\.\\COM8";
DWORD COM_BAUD_RATE = CBR_9600;
SimpleSerial Serial(com_port, COM_BAUD_RATE);

if(Serial.IsConnected()) {
    //do whatever
}
```

### Reading from Serial port
To read from the Serial port you must call the *ReadSerialPort* function. It takes two parameters, first an integer which represents the specific time the function should wait for a reply from the Ardunio in seconds, then the name of the syntax or delimiter type expected from the Arduino. It can either be the two default ones (*JSON* or *greater_less_than*), or the ones you create in the config file. Config file is described further below.

``` c++
int reply_wait_time = 7;
string syntax_type = "mine";

string incoming = Serial.ReadSerialPort(reply_wait_time, syntax_type);
```
The received string is then returned without the delimiters. If no delimiters are found in the config file, or the reading has failed for certain reason, the function will return an appropriate warning message.


### Writing to Serial port
Call this function and input your char* that you would like to write. Returns true if writing was successful.

``` c++
bool is_sent = Serial.WriteSerialPort(to_send);

if (is_sent) {
    //do whatever
}
```

It is also easy to send strings, however, you will have to declare a pointer to your string. Example code:

```c++
printf("What would you like to send?\n");
string read_in;
cin >> read_in;

char *to_send = &read_in[0];
bool is_sent = Serial.WriteSerialPort(to_send);
	
if (is_sent) {
    //do whatever
}
```

WriteSerialPort function does not append any delimiters by default. This is done due to the fact some *JSON* constructing libraries append their own delimiters. So if you choose to use a *JSON* library just put your parsed *JSON* into a string and add it as a parameter:

``` C++
string my_json = "{"name":"John"}";

char *to_send = &my_json[0];
bool is_sent = Serial.WriteSerialPort(to_send);

if (is_sent) {
    //do whatever
}
```

To check for delimiters on the Ardunio, I highly recommend using [this](http://forum.arduino.cc/index.php?topic=396450) tutorial, especially, *Example 3 - A more complete system*.

### Closing Serial port
Simple. Call this function when you would like to close the Serial port. Returns *true* if successful.
``` c++
bool closed_  = Serial.CloseSerialPort();

if(closed_) {
    //do whatever
}
```
### Creating custom syntax
There is an option to create custom delimtier syntaxes in the local config file. The config file is generated locally when the program is run for the first time. To create your own custom syntaxes you can either add it manually into the program,
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
It is rather simple to send back information with your own delimiters from the Arduino. You'll have to concatonate your message at the first index and at the last one with your delimiters:

``` C++
String my_message = "test";
String complete_msg = "{" + my_message + "}";
```

Afterwards, as mentioned before, you just create a pointer to the first index of your complete message string and write that into the Serial port:

``` C++
char *to_send = &complete_msg[0];
Serial.write(to_send);
```

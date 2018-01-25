# Simple serial communication between a Windows C++ application and an Arduino

This was created to allow a simple serial port communication integration into any c++ application that intends to talk to an Ardunio. Reading from Serial port uses delimiters which are appended at the beginning and end of each string sent from the Ardunio. Examples are provided further below.

# Features:

  - Write to Serial port
  - Read from Serial port
  - Close connection when needed


### You can also:
  - Set the waiting time for the reply from the Arduino
  - Use common delimiters
  - Add custom delimiters via a local config file

### Initialization the program
The program allows you to set the com port and the baud rate through the constructor. To use the baud rate, due to Windows API, a CBR_ must be added, as shown below.
``` c++
char com_port[] = "\\\\.\\COM8";
DWORD COM_BAUD_RATE = CBR_9600;
SimpleSerial Serial(com_port, COM_BAUD_RATE);
```

### Reading from Serial port

### Writing to Serial port
Call this function and input your char* that you would like to write. Returns true if writing was successful.

``` c++
bool is_sent = Serial.WriteSerialPort(to_send);

if (is_sent) {
    do whatever
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
    do whatever
}
```

### Closing Serial port
Simple. Call this function when you would like to close the Serial port. Returns *true* if successful.
``` c++
SimpleSerial::CloseSerialPort()
```
### Creating custom syntax
The config file is generated locally when the program is run for the first time. To create custom syntax you can either add it manually into the program,
``` c++
if (syntaxfile) {
		syntaxfile << "json { }\n";
		syntaxfile << "greater_less_than < >\n";
		syntaxfile.close();
	}	
```
or insert it directly into the sytax_config.txt file as a new line, sperated with spaces:

``` c++
json { }
greater_less_than < >	
```

### Arduino side

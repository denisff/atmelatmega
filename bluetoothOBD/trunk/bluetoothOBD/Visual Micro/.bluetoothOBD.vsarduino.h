/* 
	Editor: http://www.visualmicro.com
	        visual micro and the arduino ide ignore this code during compilation. this code is automatically maintained by visualmicro, manual changes to this file will be overwritten
	        the contents of the Visual Micro sketch sub folder can be deleted prior to publishing a project
	        all non-arduino files created by visual micro and all visual studio project or solution files can be freely deleted and are not required to compile a sketch (do not delete your own code!).
	        note: debugger breakpoints are stored in '.sln' or '.asln' files, knowledge of last uploaded breakpoints is stored in the upload.vmps.xml file. Both files are required to continue a previous debug session without needing to compile and upload again
	
	Hardware: Arduino Duemilanove w/ ATmega328, Platform=avr, Package=arduino
*/

#define __AVR_ATmega328p__
#define __AVR_ATmega328P__
#define ARDUINO 101
#define ARDUINO_MAIN
#define F_CPU 16000000L
#define __AVR__
#define __cplusplus
extern "C" void __cxa_pure_virtual() {;}

//
void waitForResponse();
char* readResponse();
//
void setupOBD2();
void setupBlueToothConnection();
void resetBT();
void enterComMode();
void enterATMode();
void sendATCommand(String command);

#include "C:\arduino-1.0.3-windows\arduino-1.0.3\hardware\arduino\variants\standard\pins_arduino.h" 
#include "C:\arduino-1.0.3-windows\arduino-1.0.3\hardware\arduino\cores\arduino\arduino.h"
#include "C:\Users\osch\Documents\Atmel Studio\6.2\bluetoothOBD\bluetoothOBD\bluetoothOBD.ino"

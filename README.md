# Basys MX3 Library Pack

BasysMX3 incorporates a wide range of modules. Digilent provides a set of libraries, allowing user 
easy access to each module’s functionality.

Basically, these libraries hide the hardware implementation details; they are wrappers over the 
lower level functions that access the registers, allowing the user to call the needed functionality 
in an easy and intuitive manner.

These libraries are designed to work in the MPLAB X IDE provided by Microchip.

## Using the Library

Normally each library has a .c and .h file. In order to use this library:

* include in your project the .c and .h files corresponding to the module you want to use (for 
 example led.c and led.h). This can be easily done by copying the files in the project folder and 
 then Add Existing Item (right mmouseclick on the project source folders and  project header files folders.
* In your code, include the header of the module
 
~~~~
#include "led.h"
~~~~

* In your code, call the needed functions, for example:

~~~~
LED_Init();  
LED_SetValue(4, 1); //turn on LED4
~~~~

Using these libraries is not required in order to access the Basys MX3 devices, of course you can write your own functionality (possibly using the libraries as inspiration).

## Additional Library Documentation

The usage of each library is documented in the library's .c and .h file. An overview of all the libraries is found in the Basys_MX3_LibPack_UG.docx Word document.

## Demos

Demos that use each of the libraries are found in the Demos folder. They are provided as MPLAB X projects. 

## Known Issues









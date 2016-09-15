# AutoUc7
Arduino code for programmatic interfacing with Leica UC 7 via mouse emulation

The Leica UC7 ultramicrotome has a touch screen interface with a usb input on the back of the machine.  A mouse can be plugged into that port, and used to control the microtome via clicking on buttons.  

This project aims to provide a programmatic way to interface with the UC7, thereby starting/stopping the microtome, and changing between cut thickness presets in an automated way.  This theoretically can allow you to coordinate the section cutting with other aspects of a microtome collection system.

# Arduino setup
I have only gotten this code working on an Arduino Leonardo, which has the advantage of being pre-setup with firmware that presents a USB HID device, and works with the built in Arduino Mouse library.

However, the firmware must be modified in one important way, in that when you program the arduino with the arduino IDE, it also presents itself to the computer as having a CDC serial interface, that can be used to communicate to a computer host connected to the USB.  This however, confuses the Leica UC-7.  If you attempt to connect it to the Leica, the Leica will pop up with a message asking what driver it should use, and of course does not have an arduino driver loaded.  It subsequently will not respond to the mouse component of the arduino device.  In order to make this work, the CDC serial driver must be disabled.  This can be done in a few ways, but I edited the USBCore.cpp file in the Arduino IDE code.  See my fork of the arduino IDE, which has only this one edit.
https://github.com/fcollman/Arduino/commit/c367990383f96d17395ee36162fc6dc19ef0ef83. 

Once you have reprogrammed the Leonardo with this code, you should no longer see a Serial Port available on your computer when you connect the Leonardo.  This adds a small complication in that you must reprogram the Leonardo while its in bootloader mode, which by default lasts for a few seconds after each reset.  So to reprogram it, simply hold the reset button, hit upload on the Arduino IDE, and then release the reset button once the IDE says "uploading". 

#Arduino program

The LeicaUC7Serial.ino file in the repo should be used to reprogram the Leonardo.

By default the sketch sets up a serial interface on Serial1 (pins 0,1).

The present software communication protocol is as follows, but can easily be expanded to cover more options.

Send: Px (x = 1-5)
Response: Px
Description: activate cut preset number x, where x=1 is the lowest most preset on the Leica screen.

Send: Yx (x = any integer)
Response: Yx
Description: Set the Delta Y step between different presets. 

HW protocol
The sketch also sets up a simple set of digital control lines for direct TTL control of the first two UC-7 presets.  We use these as section thickness 0 nm, and our production thickness (45-100nm). When Pin 2 goes high, it will bring Pin 4 low, then activate preset thickness 0 and then bring Pin 4 high, indicating that it has received the TTL signal. When Pin 3 goes high, it will ring Pin 5 low, then activate preset thickness 1 and then bring Pin 5 high, indicating that it has received the TTL signal. 

#Operation

The arduino code works simply by moving the mouse in a reproducible fashion.  First it brings the mouse to the extreme upper left, by sending a series of upward and leftward movements.  It then moves the cursor downwards and to the right to access specific pixels on the Leica screen and clicking on the buttons. It is in this sense an open loop control interface and has not yet been extensively tested for reliability

#Python interface

I have added a prototype python interface (LeicaUC7Arduino.py) to interact with the LeicaUC7Arduino, its fairly simple, but wraps the bytewise communication protocol in some friendlier methods. 

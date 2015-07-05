Simple Small Bedroom Clock
==============

See short demo
--------------
Simple Small Bedroom Clock in action on [YouTube](http://youtu.be/bC-QjfINahU)
Post on my [website](http://sasakaranovic.com/projects/build-small-bedroom-clock-from-scratch-in-under-4hours/)


[Update] Firmware v2
--------------
Since a lot of people asked me if I could adjust the firmware to have the option cycle trough hour, min, day, month, year changing. So now in firmware v2, when you press the H button, it will cycle trough hour, min, day, month, year and indicate current selection with ">" or "<". The M button will increase the selected value by 1. After you cycle trough all 6 fields, the ">" and "<" indicators will disappear and the new time/date will be set (stored in RTC).
Since this was the one day project from the begging, don't expect crystal clear comments and perfect code, but I'm sure most of you will easily understand it since nothing too complex is going on there.

Thank you for your interest in this project and like always, your feedback is greatly appreciated!


Before we start...
--------------
I wanted to make this project for myself, and since this is very simple project, I placed a challenge to complete it in shortest time possible and make it work as I intended to. 
From idea to finished product it took about 4-5 hours, including designing schematic, routing PCB and making it, programming the device and etc.
What is important is that finished product works like I want it to do, and it could be used as opportunity to learn something about other platforms different from some that are spamming us 24/7 like Arduino.
Don't get me wrong, Arduino is great and all but sometimes I want to build something for myself from scratch. :)

One thing I want to ask you all is to share your opinion, how to make this project better, what would you add/remove, did you use it? Did you like it? Did you make/modify it? Share it with us!


Structure
--------------
- Datasheets
-- Collection of datasheets of all components used in this project so you can check and see how things work and how they are set-up.
- Electronic Schematics
-- Schematic of the whole project in JPG format and PCB and Silkscreen so you can see the dimensions of the finished PCB and how components are placed around the PCB.
- Microchip XC8 Firmware
-- C source files for PIC16F1825 MCU. Containing complete MPlab X project with all source codes, libraries and etc
- Finished product images
-- Images of the finished product, this is how it looks when it's done and fired up!


What does this project have that others don't?
--------------
Well, most likely nothing. I bet all of you have seen LCD displays, RTCs and etc used in many many projects before. So this project isn't special in any way. I made it public so anyone who wants to make this clock or project similar to this one, doesn't have to start from scratch, you can use my project and edit it to suit your needs. Also as a community we can make it better for all of us.

Features
--------------
Some of the features are
- Open source
- **Very low price of components**
- Widely available components
- RTC (so you don't loose time if you power off the device for some period of time)
- Small footprint
- **You can make it by your own**
- You can customize it to look whatever you like...


Components used
--------------
Core components of this project are:
- Microchip PIC16F1825 Microcontroller
- HD44780 LCD 2x16 display
- DS1307 RTCC
- LM2596 Switching mode voltage regulator


Why did I make this project and what it is about?
--------------
So how many of you like to sleep in completely dark room? Blinds closed, no light coming in at all... Well I can sleep in whatever conditions, but my wife likes to sleep in pitch black room.
It's OK and all but it makes it impossible to see the clock on the wall. If you get digital one with back-light and place it on wall, then it's too bright. If I try to use my phone, I usually drop it from the bed/bedroom table 2-3 times before I find unlock button and wake everyone while doing it...
So what did I do? I made a small clock with back-light, that I can place by the bed, on the floor and I will be able to see what time it is and on the same time, it isn't bright enough to prevent my better half from sleeping.

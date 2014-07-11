Simple Small Bedside clock
==============

Before we start...
--------------
I wanted to make this project for myself, and since this is very simple project, I placed a challenge to complete it in shortest time possible and make it work as I intended to. 
From idea to finished product it took about 4-5 hours, including designing schematic, routing PCB and making it, programming the device and etc.
So this project may not be perfect in any way but it **works**, and could be used as oppurtunity to learn something about other platforms diferent from Arduino.
Don't get me wrong, Arduino is great and all, but why would you pay 10-15$ for Arduino + LCD and other parts, when you can build it yourself for 4-5$.

One thing I want to ask you is to share your opinion, how to make this project better, what would you add/remove, did you use it? Did you like it? Did you make/modify it? Share it with us!


Structure
--------------
- Datasheets
-- Collection of datasheets of all components used in this project so you can check and see how things work and how they are set-up.
- Electronic Schematics
-- Schematic of the whole project in JPG format and PCB and Silkscreen so you can see the dimensions of the finished PCB and how components are placed around the PCB.
- Microchip XC8 Source Files
-- C source files for PIC16F1825 MCU. Containing complete MPlab X project with all source codes, libraries and etc
- Finished product images
-- Images of the finished product, this is how it looks when it's done and fired up!


What does this project have that others dont?
--------------
Well, most likely nothing. I bet all of you have seen LCD displays, RTCs and etc used in many many projects before. So this project isn't special in any way. I made it public so anyone who wants to make this clock or project similair to this one, doesn't have to start from scratch, you can use my project and edit it to suit your needs. Also as a community we can make it better for all of us.

Features
--------------
Some of the features are
- Open source
- **Very low price of components**
- Widely available components
- RTC (so you don't loose time if you unpower the device for some period of time)
- Small footprint
- **You can make it by your own**
- You can customize it to look whatever you like...


Components used
--------------
Core components of this project are:
- Microchip PIC16F1825 microcontroller
- HD44780 LCD 2x16 display
- DS1307 RTCC
- LM2596 Switching mode voltage regulator


Why did I make this project and what it is about?
--------------
So how many of you like to sleep in completely dark room? Blinds closed, no light comming in at all... Well I can sleep in whatever conditinos, but my wife likes to sleep in pitch black room.
It's ok and all but it makes it impossible to see the clock on the wall. If you get digital one with backlight and place it on wall, then it's too bright. If I try to use my phone, I usally drop it from the bed/bedside table 2-3 times before I find unlock button and wake everyone while doing it...
So what did I doo? I made a small clock with backlight, that I can place by the bed, on the floor and I will be able to see what time it is and on the same time, it isn't bright enough to prevent my better half from sleeping.

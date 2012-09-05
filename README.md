Yet-Another-LED-Cube
====================

About
-----
An attempt to reinvent..the cube? 

Sorry, this file hasn't had much effort put into it yet :/ A ![video](https://www.dropbox.com/s/au3ys8abuv36xhp/MOV071.mp4) to hold you over for now.

![](https://raw.github.com/bradley219/Yet-Another-LED-Cube/master/misc/IMG604.jpg) 

Schematic available in PDF and Eagle formats in the \`schematic' directory
![](https://raw.github.com/bradley219/Yet-Another-LED-Cube/master/schematic/Yet-Another-LED-Cube.sch.jpg)


Hardware
--------
### LED Matrix Topography
Since I only had enough hardware to create a 4x4x4 cube, I wanted the display to be as robust as
possible. I opted to use the TLC5940 16-channel constant-current LED driver from Texas
Instruments, which provides 16 independent 12-bit PWM channels, each of which can sink &gt;100mA 
of current. My goal was to be able to use a single 16-channel driver to drive all 192 channels by
using a multiplexing scheme. To acheive this, I grouped the LEDs into individual "pages" of 4 LEDs.
The LEDs within a "page" share a common anode; cathodes are shared among LEDs in corresponding
positions within other pages.

### Drive Theory and Timing
![](https://raw.github.com/bradley219/Yet-Another-LED-Cube/master/misc/led-page-unit.png)
The design of the cube allows for only a single "page" of LEDs to be driven at any given moment. The
16 pages which make up the cube are driven in sequence at about 100Hz (full-cycle period). 

### High-Side Switching
### Low-Side Switching/Current Sink

Software
--------

### Driver
### Driver Interface
### Color Calculations
### Animations

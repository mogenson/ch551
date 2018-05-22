# Example project for the WCH CH551 8051 microcontroller using CMAKE and SDCC

This example program blinks the LED connected to pin 0 of GPIO port 3 at 2 Hz.
There's no external header files, register defines, or libraries required to
build. This project uses [CMake](https://cmake.org/) for the build system and
[SDCC](http://sdcc.sourceforge.net/) for the compiler.

## Instructions

1. Buy a CH551 development board from http://www.electrodragon.com/product/ch551-mini-dev-board-ch55x-series/
2. Install CMake and SDCC
3. Build and install `wchisptool` from https://github.com/rgwan/librech551
4. Generate makefile: (optionally use ccmake or cmake-gui to configure project)
    ```
    $ cd build
    $ cmake ..
    ```
5. Build and flash program:
    ```
    $ make
    $ make flash
    $ make erase
    $ make clean
    ```

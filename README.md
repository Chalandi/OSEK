OSEK_cm3
==================

This repository implements an OSEK-like, bare-metal
operating system for ARM(R) Cortex(R)-M3.

The goal is near-compatibility with OSEK (ECC1/BCC1) Implementation for ARM(R) Cortex(R)-M3

Features include:
  - OSEK-like OS implementation with support of most common features.
  - Sample application featuring tasks and events interacting to produce blinky at 1/2 Hz on the user LED.
  - Bare-metal startup using the RPI's standard `kernel.img` file on SD card.
  - Power and port initialization.
  - 1ms timebase derived from the `SysTick`.
  - Implementation in C99 with absolute minimal use of assembly.

A clear and easy-to-understand build system based on KEIL MDK
completes this fun and educational project.

This repository provides keen insight on writing your own
_bare_ _metal_ operating system from scratch on a modern
microcontroller using entirely free tools and open standards.

## Details on the Application

Following low-level chip initialization, the program jumps
to the `main()` subroutine in [Application/Appli.c](./Application/Appli.c).
Here the single functional line in `main()`
starts the operatng system via call to `OS_StartOS()`.

The sample application runs on the STM32F100-NUCLEO Board.

An idle task and one single extended task animate the user LED,
providing a simple blinky LED show featuring the gren and blue
LED(s) toggling at 1Hz (green) and 1/2 Hz (blue) respectively.

## Building the Application

Build on `Win*` is easy using an installed KEIL-MDK

# OSEK

TBD

## References
Further information on open standard OSEK can be found in ISO 17356 and in the link below:
* https://en.wikipedia.org/wiki/OSEK

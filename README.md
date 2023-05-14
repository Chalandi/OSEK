chalandi/OSEK
==================

This repository implements an OSEK-like, bare-metal
operating system for ARM(R) Cortex(R)-M3.
The goal is to achieve near-compatibility with OSEK (ECC1/BCC1).

Features include:
  - OSEK-like OS implementation with support of most common features.
  - The sample application runs on the STM32F100-NUCLEO Board.
  - Sample application with tasks and events interacting to produce blinky LED show featuring the gren and blue LEDs toggling at 1Hz (green) and 1/2 Hz (blue) respectively.
  - Use a simple self-written bare-metal startup.
  - Power, clock and port initialization.
  - 1ms timebase derived from the ARM(R) `SysTick`.
  - Implementation in C99 with absolute minimal use of assembly.

A clear and easy-to-understand build system based on KEIL MDK
completes this fun and educational project.

This repository provides keen insight on writing your own
_bare_ _metal_ operating system from scratch on a modern
microcontroller using entirely free tools and open standards.

## Details on the Application

Following low-level chip initialization, the program jumps
to the `main()` subroutine in [Application/Appli.c](./Application/Appli.c).
Here there are two functional lines. The first line initializes the LEDs.
The second line subsequently starts the operatng system via call
to `OS_StartOS()`.

An idle task and one single extended task animate the user LED,
providing a simple blinky LED show featuring the gren and blue
LED(s) toggling at 1Hz (green) and 1/2 Hz (blue) respectively.

## Building the Application

### Build with KEIL uVision (MDK)

Build on `Win*` is easy using an installed KEIL-MDK.
Simply use the project file `PRJ_02.uvprojx` which can
be found in the project's root directory.

### Build with GNUmake on `*nix`

Build on `*nix*` is easy using an installed `gcc-arm-none-eabi`

```sh
cd OSEK
./Build.sh
```

The build results including ELF-file, HEX-mask, MAP-file
can be found in the `Output` directory following the GNUmake build.

## References
Further information on open standard OSEK can be found in ISO 17356 and in the link below:
* https://en.wikipedia.org/wiki/OSEK

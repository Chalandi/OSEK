/******************************************************************************************
  Filename    : Memory_Map.ld

  Core        : STM32F100RB ARM(R) Cortex(R)-M3

  Author      : Kormanyos Christopher

  Owner       : Chalandi Amine

  Date        : 14.05.2023

  Description : Linker description file for ARM1176JZF-S(ARMv6)

******************************************************************************************/

/******************************************************************************************
ELF Entrypoint
******************************************************************************************/
ENTRY(SysStartup_Init)

INPUT(libc.a libgcc.a)

OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)

/******************************************************************************************
 Internal flash configuration
******************************************************************************************/

__initial_stack_pointer = 0x20002000 - 4;

MEMORY
{
  VEC(rx)  : ORIGIN = 0x08000000, LENGTH = 0x200
  ROM(rx)  : ORIGIN = 0x08000200, LENGTH = 32K - 0x200
  RAM(rwx) : ORIGIN = 0x20000000, LENGTH = 0x1800
}

/******************************************************************************************
 Sections definition
******************************************************************************************/
SECTIONS
{
  /* Interrupt vector table */
  /* ISR vectors */
  .isr_vector :
  {
    *(.isr_vector)
    . = ALIGN(0x100);
    KEEP(*(.isr_vector))
  } > VEC = 0xAAAA

  /* Program code (text) */
  .text :
  {
    *(.progmem*)
    . = ALIGN(4);
    *(.text)
    . = ALIGN(4);
    *(.text*)
    . = ALIGN(4);
    *(.rodata)
    . = ALIGN(4);
    *(.rodata*)
    . = ALIGN(4);
    *(.glue_7)
    . = ALIGN(4);
    *(.glue_7t)
    . = ALIGN(4);
  } > ROM

  /* Section for constructors */
  .ctors :
  {
    __CTOR_LIST__ = . ;
    LONG((__CTOR_END__ - __CTOR_LIST__) / 4 - 2);
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
    KEEP (*(SORT(.init_array.*)))
    KEEP (*(.init_array))
    LONG(0) ;
    __CTOR_END__ = . ;
    . = ALIGN(4);
  } > ROM

  /* Runtime clear table */
  .clear_sec :
  {
    . = ALIGN(4);
    PROVIDE(__RUNTIME_CLEAR_TABLE = .) ;
    LONG(0 + ADDR(.bss));   LONG(SIZEOF(.bss));
    LONG(-1);               LONG(-1);
  } > ROM

  /* Runtime copy table */
  .copy_sec :
  {
    . = ALIGN(4);
    PROVIDE(__RUNTIME_COPY_TABLE = .) ;
    LONG(LOADADDR(.data));  LONG(0 + ADDR(.data));  LONG(SIZEOF(.data));
    LONG(-1);               LONG(-1);               LONG(-1);
  } > ROM

  .ARM.extab :
  {
    . = ALIGN(4);
    *(.ARM.extab)
    *(.gnu.linkonce.armextab.*)
    . = ALIGN(4);
  } > ROM

  .exidx :
  {
    . = ALIGN(4);
    PROVIDE(__exidx_start = .);
    *(.ARM.exidx*)
    . = ALIGN(4);
    PROVIDE(__exidx_end = .);
  } > ROM

  .ARM.attributes :
  {
    *(.ARM.attributes)
  } > ROM

  /* The ROM-to-RAM initialized data section */
  .data :
  {
    *(.data)
    . = ALIGN(4);
  } > RAM AT > ROM

  /* The uninitialized (zero-cleared) bss section */
  .bss :
  {
    *(.bss)
    . = ALIGN(4);
  } > RAM
}

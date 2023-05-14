// *****************************************************************************
// Filename    : HwPltfm.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : Hardware Platform definition for ARM Cortex-M3
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef __HARDWARE_PLATFORM_H__
#define __HARDWARE_PLATFORM_H__

#if 0
#define ENABLE_IRQ(x)(*(volatile unsigned int*)(0xE000E100)) |= 1<<x 
#define TRAP(x)      (*(volatile unsigned int*)(0xE000EF00)) = x
#define OS_DISPATCH() ENABLE_IRQ(6);TRAP(6)
#endif

#define SCB_ICSR 0xE000ED04UL
#define SET_PENDSV()   (*(volatile unsigned int*)(SCB_ICSR)) |= 1<<28
#define CLEAR_PENDSV() (*(volatile unsigned int*)(SCB_ICSR))
#define OS_DISPATCH() SET_PENDSV()	

#if defined(__GNUC__)
#define ENABLE_INTERRUPTS()  __asm("CPSIE I")
#define DISABLE_INTERRUPTS() __asm("CPSID I")
#else
#define ENABLE_INTERRUPTS()	  __asm{CPSIE I}
#define DISABLE_INTERRUPTS()	__asm{CPSID I}
#endif

#define EXCEPTION_CODE_MASK   (uint32)0x1F


#endif

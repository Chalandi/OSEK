// *****************************************************************************
// Filename    : SysTickTimer.h
// 
// Core        : STM32F100RB Cortex™-M3  
// 
// Board       : STM32VLDISCOVERY
//
// Compiler    : ARM® Compiler v5.06 for μVision (Keil)
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 04.09.2017
// 
// Description : System tick timer header file
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************
#ifndef __SYSTICKTIMER_H__
#define __SYSTICKTIMER_H__

#include"types.h"

#define SYS_TICK_BASE_REG (unsigned int)(0xE000E010)
	
#define STK_CTRL   *((unsigned int*)(SYS_TICK_BASE_REG + 0x00))	
#define STK_LOAD   *((unsigned int*)(SYS_TICK_BASE_REG + 0x04))	
#define STK_VAL    *((unsigned int*)(SYS_TICK_BASE_REG + 0x08))	
#define STK_CALIB  *((unsigned int*)(SYS_TICK_BASE_REG + 0x0C))
	
#define STK_CTRL_PTR  ((unsigned int*)(SYS_TICK_BASE_REG + 0x00))

typedef struct
{
	unsigned int Enable:1;
	unsigned int TickInt:1;
	unsigned int ClockSrc:1;
	unsigned int :13;
	unsigned int CountFlag:1;
	unsigned int :15;	
}stStkCtrl;

#define SYS_TICK_CLKSRC_AHB   1
#define SYS_TICK_ENABLE_INT   1
#define SYS_TICK_ENABLE_TIMER 1
#define SYS_TICK_1MS          (unsigned int)0x5DC0 - 1

void SysTickTimer_Init(void);
void SysTickTimer_Start(void);

#endif

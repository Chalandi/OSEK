// *****************************************************************************
// Filename    : SysTickTimer.c
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
// Description : System tick timer Implementation
//
// License     : GNU General Public License v3.0
//
// *****************************************************************************
#include"SysTickTimer.h"



stStkCtrl* StkCtrl = (stStkCtrl*)STK_CTRL_PTR;


void SysTickTimer_Init(void)
{
	STK_CTRL = 0;
	STK_LOAD = SYS_TICK_1MS;
	STK_VAL  = 0;
	StkCtrl->ClockSrc = SYS_TICK_CLKSRC_AHB;
	StkCtrl->TickInt = SYS_TICK_ENABLE_INT;	
}

void SysTickTimer_Start(void)
{
	StkCtrl->Enable = SYS_TICK_ENABLE_TIMER;
}

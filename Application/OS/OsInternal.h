// *****************************************************************************
// Filename    : OsInternal.h
// 
// OS          : OSEK 2.2.3  
// 
// CC          : ECC1/BCC1
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OS internal services
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef __OS_INTERNAL_H__
#define __OS_INTERNAL_H__

#include"types.h"

extern void OsGetCurrentSP(volatile unsigned int* CurrentSpPtr);
extern void OsGetPSR(volatile unsigned int* CurrentPsr);

boolean OsIsInterruptContext(void);
boolean OsIsCat2IntContext(void);

#endif

// ******************************************************************************************************
// Filename    : TCB.c
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
// Description : Task Control Block definition
//
// License     : GNU General Public License v3.0
//  
// ******************************************************************************************************

#include"TCB.h"

#if !defined(__GNUC__)
#pragma diag_suppress 1296,1,191,188
#endif

/***************************************************/
/*            X Macro System Generation            */
/***************************************************/

#define OS_GEN_DATA_STRUCT
#include"OsCfg.h"
#undef OS_GEN_DATA_STRUCT

#define OS_GEN_TCB
#include"OsCfg.h"
#undef OS_GEN_TCB

#define OS_GEN_OCB
#include"OsCfg.h"
#undef OS_GEN_OCB

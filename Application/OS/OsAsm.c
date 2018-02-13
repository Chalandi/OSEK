// *****************************************************************************************************************
// Filename    : OsAsm.c
// 
// Core        : Cortex™-M3
//
// Compiler    : ARM® Compiler v5.06 for µVision (Keil)
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : OSEK Context switch and ISR category 2 wrapper
//
// License     : GNU General Public License v3.0
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsDispatcher
///
/// \descr  Context switcher
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
__asm void OsDispatcher(void)
{
  PRESERVE8 {TRUE}
  extern OS_Dispatcher

  cpsid i              /* Lock the dispatcher                       */
  push {r4 - r11, lr}  /* Store the current context into the stack  */
  mov r0,r13           /* Store the current stack pointer           */
  bl.w OS_Dispatcher   /* Call the dispatcher to switch the context */
  mov r13,r0           /* Setup the new stack pointer               */
  pop {r4 - r11, lr}   /* Restore the saved context                 */
  cpsie i	             /* Unlock the dispatcher                     */
  bx lr
}


//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetCurrentSP
///
/// \descr  Get the current stack pointer register value
///
/// \param  volatile unsigned int* CurrentSpPtr (out): Current stack pointer register value
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
__asm void OsGetCurrentSP(volatile unsigned int* CurrentSpPtr)
{
		str r13,[r0]
	  bx lr
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetPSR
///
/// \descr  Get the program status register value
///
/// \param  volatile unsigned int* CurrentPsr (out): Current program status register value
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
__asm void OsGetPSR(volatile unsigned int* CurrentPsr)
{
		mrs r1, psr
	  str r1,[r0]
	  bx lr
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsCat2IsrWrapper
///
/// \descr  Wrapper for all category 2 interrupts
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
__asm void OsCat2IsrWrapper(void)
{
	PRESERVE8 {TRUE}
	extern OsStoreStackPointer
	extern OsGetSavedStackPointer
	extern OsIsrCallDispatch
  extern OsRunCat2Isr		

#ifndef OS_NESTED_INT	
	cpsid i
#endif	
	push {r4 - r11, lr}	         /* Save the context in the stack of the current task                */
	mov r0,r13                   /* prepare the input parameter for the function OsStoreStackPointer */
	bl.w OsStoreStackPointer     /* Save the stack pointer of the current task                       */
	bl.w OsRunCat2Isr            /* Call the ISR (lookup table)                                      */
	bl.w OsGetSavedStackPointer  /* Restore the stack pointer of the current task                    */
	bl.w OsIsrCallDispatch       /* Call dispatcher if needed                                        */
	mov r13,r0                   /* Set the new stack pointer of the active task                     */
	pop {r4 - r11, lr}           /* Restore the context from the active task                         */
#ifndef OS_NESTED_INT
	cpsie i
#endif	
	bx lr	
}	


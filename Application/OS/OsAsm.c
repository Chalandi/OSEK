// *****************************************************************************************************************
// Filename    : OsAsm.c
// 
// Core        : Cortex(R)-M3
//
// Compiler    : ARM(R) Compiler v5.06 for uVision (Keil)
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

#if defined(__GNUC__) && !defined(__CC_ARM)
#include "types.h"
#elif defined(__CC_ARM)
#else
#error Error: Compiler inline assembly dialect is not supported
#endif

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsDispatcher
///
/// \descr  Context switcher
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((naked, used, noinline)) void OsDispatcher(void)
#elif defined(__CC_ARM)
__asm void OsDispatcher(void)
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
{

#if defined(__GNUC__) && !defined(__CC_ARM)
  extern uint32 OS_Dispatcher(uint32);
  __asm("cpsid i");              /* Lock the dispatcher                       */
  __asm("push {r4 - r11, lr}");  /* Store the current context into the stack  */
  __asm("mov r0,r13");           /* Store the current stack pointer           */
  __asm("bl.w OS_Dispatcher");   /* Call the dispatcher to switch the context */
  __asm("mov r13,r0");           /* Setup the new stack pointer               */
  __asm("pop {r4 - r11, lr}");   /* Restore the saved context                 */
  __asm("cpsie i");              /* Unlock the dispatcher                     */
  __asm("bx lr");
#elif defined(__CC_ARM)
  extern OS_Dispatcher
  PRESERVE8 {TRUE}
  cpsid i              /* Lock the dispatcher                       */
  push {r4 - r11, lr}  /* Store the current context into the stack  */
  mov r0,r13           /* Store the current stack pointer           */
  bl.w OS_Dispatcher   /* Call the dispatcher to switch the context */
  mov r13,r0           /* Setup the new stack pointer               */
  pop {r4 - r11, lr}   /* Restore the saved context                 */
  cpsie i              /* Unlock the dispatcher                     */
  bx lr
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
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
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((naked, used, noinline)) void OsGetCurrentSP(volatile unsigned int* CurrentSpPtr)
#elif defined(__CC_ARM)
__asm void OsGetCurrentSP(volatile unsigned int* CurrentSpPtr)
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
{
#if defined(__GNUC__) && !defined(__CC_ARM)
  (void) CurrentSpPtr;
  __asm("str r13,[r0]");
  __asm("bx lr");
#elif defined(__CC_ARM)
  str r13,[r0]
  bx lr
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
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
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((naked, used, noinline)) void OsGetPSR(volatile unsigned int* CurrentPsr)
#elif defined(__CC_ARM)
__asm void OsGetPSR(volatile unsigned int* CurrentPsr)
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
{
#if defined(__GNUC__) && !defined(__CC_ARM)
  (void) CurrentPsr;
  __asm ("mrs r1, psr");
  __asm ("str r1,[r0]");
  __asm ("bx lr");
#elif defined(__CC_ARM)
  mrs r1, psr
  str r1,[r0]
  bx lr
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
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
#if defined(__GNUC__) && !defined(__CC_ARM)
__attribute__((naked, used, noinline)) void OsCat2IsrWrapper(void)
#elif defined(__CC_ARM)
__asm void OsCat2IsrWrapper(void)
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
{
#if defined(__GNUC__) && !defined(__CC_ARM)
  extern void OsStoreStackPointer(uint32);
  extern uint32 OsGetSavedStackPointer(void);
  extern uint32 OsIsrCallDispatch(uint32);
  extern void OsRunCat2Isr(void);

  #ifndef OS_NESTED_INT
  __asm("cpsid i");
  #endif
  __asm("push {r4 - r11, lr}");          /* Save the context in the stack of the current task                */
  __asm("mov r0,r13");                   /* prepare the input parameter for the function OsStoreStackPointer */
  __asm("bl.w OsStoreStackPointer");     /* Save the stack pointer of the current task                       */
  __asm("bl.w OsRunCat2Isr");            /* Call the ISR (lookup table)                                      */
  __asm("bl.w OsGetSavedStackPointer");  /* Restore the stack pointer of the current task                    */
  __asm("bl.w OsIsrCallDispatch");       /* Call dispatcher if needed                                        */
  __asm("mov r13,r0");                   /* Set the new stack pointer of the active task                     */
  __asm("pop {r4 - r11, lr}");           /* Restore the context from the active task                         */
  #ifndef OS_NESTED_INT
  __asm("cpsie i");
  #endif
  __asm("bx lr");

#elif defined(__CC_ARM)
  PRESERVE8 {TRUE}
  extern OsStoreStackPointer
  extern OsGetSavedStackPointer
  extern OsIsrCallDispatch
  extern OsRunCat2Isr

  #ifndef OS_NESTED_INT
  cpsid i
  #endif
  push {r4 - r11, lr}          /* Save the context in the stack of the current task                */
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
#else
#error Error: Compiler inline assembly dialect is not supported
#endif
}

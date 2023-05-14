// *****************************************************************************************************************
// Filename    : OS.c
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
// Description : System services implementation
//
// License     : GNU General Public License v3.0
// 
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"OsAPIs.h"
#include"TCB.h"
#include"SysTickTimer.h"


//------------------------------------------------------------------------------------------------------------------
// Static function prototypes
//------------------------------------------------------------------------------------------------------------------
static void OS_InitTimer(void);
static void OS_StartTimer(void);
static void OS_IdleLoop(void);
static void OsCreateNewContext(uint32 StackFramePtr, pFunc TaskFuncPtr);

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_StartOS
///
/// \descr  The user can call this system service to start the operating system in a specific mode
///
/// \param  OsAppModeType Mode
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_StartOS(OsAppModeType Mode)
{
	(void) Mode;
	
	if(TRUE == OsIsInterruptContext())
	{
		return;
	} 	
	
	if(NB_OF_TASKS > 0)
	{		
		/* INIT TCBs */
		for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
		{
			/* Init all stacks with marker 0xAA */
			const uint32 stack_size = OCB_Cfg.pTcb[tcbIdx]->pstack_top - OCB_Cfg.pTcb[tcbIdx]->pstack_bot + sizeof(uint32);
			for(uint32 Idx=0u;Idx < stack_size;Idx++)
			{
				*(uint32*)((OCB_Cfg.pTcb[tcbIdx]->pstack_bot) + Idx) = (uint32)0xAAAAAAAAu;
			}
			
			/* Set default tasks priorities */
			OCB_Cfg.pTcb[tcbIdx]->Prio = OCB_Cfg.pTcb[tcbIdx]->FixedPrio;
		}
		
		/* Init system tick timer */
		OS_InitTimer();
		
		/* Start all autostart task */
		for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
		{
			if(OCB_Cfg.pTcb[tcbIdx]->Autostart == OS_AUTOSTART && OCB_Cfg.pTcb[tcbIdx]->TaskStatus == SUSPENDED)
			{
				/* Switch to PRE_READY state*/
				OCB_Cfg.pTcb[tcbIdx]->TaskStatus = PRE_READY;
				
				/* Update number of activation */
				OCB_Cfg.pTcb[tcbIdx]->NbOfActiv--;
			}
		}
		
		/* Call startup hook function */
		#if(STARTUPHOOK)
		StartupHook();
		#endif
		
		/* Start system tick timer */
		OS_StartTimer();
				
		/* Lock the dispatcher */
		OCB_Cfg.OsLockDispatcher = 1;
		
		/* Call Scheduler */
		(void)OS_Schedule();
		
		/* Unlock the dispatcher */
		OCB_Cfg.OsLockDispatcher = 0;
		
		/* Save the system stack ptr */
		OsGetCurrentSP(&OCB_Cfg.OsSystemStackPtr);		

		/* Call the dispatcher */
		OS_DISPATCH();
	}
	
	/* Infinite loop */
	for(;;);
}


//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_Schedule
///
/// \descr  If a higher-priority task is ready, the internal resource of the task is released, 
///         the current task is put into the ready state, its context is saved and the higher-priority task
///         is executed. Otherwise the calling task is continued.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_Schedule(void)
{
	if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS && 
     OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->CeilingPrio != 0 &&
		 OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio != OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->FixedPrio &&
		 FALSE == OsIsCat2IntContext())
	{
		return(E_OS_RESOURCE);
	}
	else if(TRUE == OsIsInterruptContext() && FALSE == OsIsCat2IntContext()) /* Cat1 Interrupt */
	{
		return(E_OS_CALLEVEL);
	} 
	else
	{	
		sint32 HighPrio = -1;
		OCB_Cfg.HighPrioReadyTaskIdx = INVALID_TASK;
		
    /* Starting a critical section */
    DISABLE_INTERRUPTS();

		for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
		{
			if(OCB_Cfg.pTcb[tcbIdx]->TaskStatus == PRE_READY || 
         OCB_Cfg.pTcb[tcbIdx]->TaskStatus == READY     || 
         OCB_Cfg.pTcb[tcbIdx]->TaskStatus == RUNNING)
			{
				if((sint32)OCB_Cfg.pTcb[tcbIdx]->Prio > (sint32)HighPrio)
				{	
					HighPrio = OCB_Cfg.pTcb[tcbIdx]->Prio;
					OCB_Cfg.HighPrioReadyTaskIdx = tcbIdx;
				}
			}
		}
		
		/* Ending the critical section */
    ENABLE_INTERRUPTS();

		if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS && OCB_Cfg.HighPrioReadyTaskIdx < NB_OF_TASKS)
		{
			if((sint32)HighPrio > (sint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio) || 
				 OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == WAITING             || 
				 OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == SUSPENDED
				)
			{
				/* Call PostTaskHook */
				#if(POSTTASKHOOK)
				PostTaskHook();
				#endif

				if(OCB_Cfg.OsIsrInterruptLevel == 0 && OCB_Cfg.OsLockDispatcher == 0)
				{
					/* Call the dispatcher */
					OS_DISPATCH();
				}
				else if(OCB_Cfg.OsLockDispatcher == 1)
				{
					return(E_OK);
				}
				else
				{
					/*  OS_Schedule is called in interrupt context  */
					/*  the Dispatcher will be executed later after */
					/*  the execution of the ISR                    */
					OCB_Cfg.OsIsrCallDispatcher = 1;
				}
			}		
		}
		else
		{
			/* No task is active or ready */
      /* Kernel will switch to system stack and run OS_IdleLoop */

      /* Call PostTaskHook */
			if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
			{
				#if(POSTTASKHOOK)
				PostTaskHook();
				#endif
			}
      			
			if(OCB_Cfg.OsIsrInterruptLevel == 0)
			{
        /* OS_Schedule is called outside an interrupt context */
        /* Call the dispatcher */
				OS_DISPATCH();
			}
			else
			{
				/*  OS_Schedule is called in interrupt context  */
				/*  the Dispatcher will be executed later after */
				/*  the execution of the ISR                    */
				OCB_Cfg.OsIsrCallDispatcher = 1;
			}
		}
		return(E_OK);
	}	
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_InitTimer
///
/// \descr  Init the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OS_InitTimer(void)
{
	SysTickTimer_Init();	
}	

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_StartTimer
///
/// \descr  Start the HW timer used as system tick timer
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OS_StartTimer(void)
{
	SysTickTimer_Start();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_Dispatcher
///
/// \descr  Context switch engine
///
/// \param  uint32 StackPtr: Current stack pointer of the active task
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
uint32 OS_Dispatcher(uint32 StackPtr)
{

	/* Save the current stack pointer of the running task before switching the context */
	if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
	{
		OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = StackPtr;
	}

  /* Set the new current task */
	OCB_Cfg.CurrentTaskIdx = OCB_Cfg.HighPrioReadyTaskIdx;
	
	if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)	
	{	
		/* check if we need to create a new stack frame for the new task */
		if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == PRE_READY)
		{

      /* cupdate the current task state */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = RUNNING;
			
			/* Call PreTaskHook */
			#if(PRETASKHOOK)
			PreTaskHook();
			#endif
			
			/* Create Stack Frame for the 1st execution */
			uint32 NewStackFramePtr = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_top;
			pFunc	 NewThread = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->function;
			
			/* CREATE NEW CONTEXT */
			OsCreateNewContext(NewStackFramePtr, NewThread);
			
			/* Calculate the new stack ptr */
			NewStackFramePtr = (uint32)((uint32)(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pstack_top) - (uint32)0x40);	
			
			/* Save the new stack ptr */		
			OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = NewStackFramePtr;
			
		}
		else if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus == READY)
		{
			OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = RUNNING;
			
			/* Call PreTaskHook */
			#if(PRETASKHOOK)
			PreTaskHook();
			#endif
		}
	}
	else
	{
		/* Create new context for os Idle loop and switch to it using the system stack */
		OsCreateNewContext((uint32)OCB_Cfg.OsSystemStackPtr, OS_IdleLoop);

		return(OCB_Cfg.OsSystemStackPtr - 0x40);
	}		
	return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsCreateNewContext
///
/// \descr  This function create and fill the new task context
///
/// \param  uint32 StackFramePtr: Current stack pointer of the active task
///            pFunc TaskFuncPtr: task function pointer
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OsCreateNewContext(uint32 StackFramePtr, pFunc TaskFuncPtr)
{
	*(volatile uint32*)(StackFramePtr - 0x00) = (uint32)0x01000000;   //xPSR
	*(volatile uint32*)(StackFramePtr - 0x04) = (uint32)TaskFuncPtr;  //PC
	*(volatile uint32*)(StackFramePtr - 0x08) = (uint32)TaskFuncPtr;  //LR
	*(volatile uint32*)(StackFramePtr - 0x0C) = (uint32)0x00;	        //r12
	*(volatile uint32*)(StackFramePtr - 0x10) = (uint32)0x00;         //r3
	*(volatile uint32*)(StackFramePtr - 0x14) = (uint32)0x00;         //r2
	*(volatile uint32*)(StackFramePtr - 0x18) = (uint32)0x00;         //r1
	*(volatile uint32*)(StackFramePtr - 0x1C) = (uint32)0x00;         //r0
	*(volatile uint32*)(StackFramePtr - 0x20) = (uint32)0xFFFFFFF9;   //LR --> Return from TRAP	
	*(volatile uint32*)(StackFramePtr - 0x24) = (uint32)0x00;         //r11
	*(volatile uint32*)(StackFramePtr - 0x28) = (uint32)0x00;         //r10
	*(volatile uint32*)(StackFramePtr - 0x2C) = (uint32)0x00;         //r9
	*(volatile uint32*)(StackFramePtr - 0x30) = (uint32)0x00;         //r8
	*(volatile uint32*)(StackFramePtr - 0x34) = (uint32)0x00;         //r7
	*(volatile uint32*)(StackFramePtr - 0x38) = (uint32)0x00;         //r6
	*(volatile uint32*)(StackFramePtr - 0x3C) = (uint32)0x00;         //r5
	*(volatile uint32*)(StackFramePtr - 0x40) = (uint32)0x00;         //r4			
}	


//------------------------------------------------------------------------------------------------------------------
/// \brief  Interrupt service routine
///
/// \descr  ISR of system tick interrupt
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
ISR(SysTickTimer)	
{
	OCB_Cfg.OsSysTickCounter++;
	
	for(int AlarmID =0; AlarmID < NB_OF_ALARMS; AlarmID++)
	{
		if(OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint <= (uint32)OCB_Cfg.OsSysTickCounter)
		{
			/* Update Timers */
			if(OCB_Cfg.pAlarm[AlarmID]->AlarmCategory == ALARM_RELATIVE &&  OCB_Cfg.pAlarm[AlarmID]->Alarmtype == ALARM_ONE_SHOT)
			{
				OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = 0;
			}
			else if(OCB_Cfg.pAlarm[AlarmID]->AlarmCategory == ALARM_RELATIVE &&  OCB_Cfg.pAlarm[AlarmID]->Alarmtype == ALARM_CYCLIC)
			{
				OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = OCB_Cfg.pAlarm[AlarmID]->InitTicks + OCB_Cfg.pAlarm[AlarmID]->InitCycles + (uint32)OCB_Cfg.OsSysTickCounter;
			}
			else if(OCB_Cfg.pAlarm[AlarmID]->AlarmCategory == ALARM_ABSOLUTE &&  OCB_Cfg.pAlarm[AlarmID]->Alarmtype == ALARM_ONE_SHOT)
			{
				OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = 0;
			}
			else if(OCB_Cfg.pAlarm[AlarmID]->AlarmCategory == ALARM_ABSOLUTE &&  OCB_Cfg.pAlarm[AlarmID]->Alarmtype == ALARM_CYCLIC)
			{
				OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = OCB_Cfg.pAlarm[AlarmID]->InitCycles;
			}
			else
			{
				/* Do Nothing */
			}
			
			/* Execute Action */
			if(OCB_Cfg.pAlarm[AlarmID]->Action == ALARM_SET_EVENT)
			{
				OS_SetEvent((OsTaskType)OCB_Cfg.pAlarm[AlarmID]->TaskId, (OsEventMaskType)OCB_Cfg.pAlarm[AlarmID]->Event);
			}
			else if(OCB_Cfg.pAlarm[AlarmID]->Action == ALARM_ACTIVE_TASK)
			{
				OS_ActivateTask((OsTaskType)OCB_Cfg.pAlarm[AlarmID]->TaskId);
			}
			else if(OCB_Cfg.pAlarm[AlarmID]->Action == ALARM_CALLBACK)
			{
				if(OCB_Cfg.pAlarm[AlarmID]->CallBackFunc != (void*)0)
				{
					OCB_Cfg.pAlarm[AlarmID]->CallBackFunc();
				}
				else
				{
					/* NULL function pointer -> HookError could be called */
				}
			}			
			else
			{
				/* Do nothing */
			}
		}
	}
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsStoreStackPointer
///
/// \descr  Store the current stack pointer in case of category 2 interrupt
///
/// \param  uint32 StackPtrValue: Current stack pointer
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsStoreStackPointer(uint32 StackPtrValue)
{
	OCB_Cfg.OsIsrInterruptLevel = 1;
	
	if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
	{	
		OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer = StackPtrValue;
	}
	else
	{
		OCB_Cfg.OsCurrentSystemStackPtr = StackPtrValue;
	}		
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsGetSavedStackPointer
///
/// \descr  return the saved stack pointer in case of category 2 interrupt
///
/// \param  void
///
/// \return uint32 : saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 OsGetSavedStackPointer(void)
{
	OCB_Cfg.OsIsrInterruptLevel = 0;
	
	if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
	{
		return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
	}
	else
	{
		return(OCB_Cfg.OsCurrentSystemStackPtr);
	}
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsrCallDispatch
///
/// \descr  Call the dispatcher to switch the context if needed after an category 2 interrupt 
///
/// \param  uint32 StackPtr: Current stack pointer
///
/// \return uint32 : The new stack pointer after switching the context otherwise the last saved stack pointer
//------------------------------------------------------------------------------------------------------------------
uint32 OsIsrCallDispatch(uint32 StackPtr)
{
	if(OCB_Cfg.OsIsrCallDispatcher == 1)
	{
		OCB_Cfg.OsIsrCallDispatcher = 0;
		return(OS_Dispatcher(StackPtr));
	}
	if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
	{	
		return(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->pCurrentStackPointer);
	}
	else
	{
		return(StackPtr);
	}
}	

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetActiveApplicationMode
///
/// \descr  This service returns the current application mode.
///
/// \param  void
///
/// \return OsAppModeType : The application mode
//------------------------------------------------------------------------------------------------------------------
OsAppModeType OS_GetActiveApplicationMode(void)
{
	return(APP_MODE);
}	

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ShutdownOS
///
/// \descr  The user can call this system service to abort the overall system (e.g. emergency off). 
///         The operating system also calls this function internally, 
///         if it has reached an undefined internal state and is no longer ready to run.
///
/// \param  OsStatusType Error: Error causing the shutdown of the OS
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ShutdownOS(OsStatusType Error)
{

#if(SHUTDOWNHOOK)
	ShutdownHook(Error);
#else
	(void)Error;	
#endif
	
	DISABLE_INTERRUPTS();
	
	/* Kill all tasks */
	for(int tcbIdx = 0; tcbIdx < NB_OF_TASKS; tcbIdx++)
	{
		OCB_Cfg.pTcb[tcbIdx]->TaskStatus = SUSPENDED;
	}
	for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_IdleLoop
///
/// \descr  This function is called by the dispatcher when no active task is found.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
static void OS_IdleLoop(void)
{
	/* No active task found */
	for(;;);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsRunCat2Isr
///
/// \descr  This function is the entry point of all category 2 interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OsRunCat2Isr(void)
{
	uint32 CurrentPsr = 0;
	
	/* Get program status register */
	OsGetPSR(&CurrentPsr);	
	
	for(uint32 IsrIdx=0; IsrIdx < NB_OF_ISR; IsrIdx++)
	{
		if(IsrLookupTable[IsrIdx].IsrNum == (uint32)(CurrentPsr & EXCEPTION_CODE_MASK))
		{
			IsrLookupTable[IsrIdx].IsrFunc();
			return;
		}
	}
}	

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsInterruptContext
///
/// \descr  This function check the cpu mode (thread or interrupt).
///
/// \param  void
///
/// \return boolean: TRUE -> Cpu in interrupt context, FALSE -> Cpu not in interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean OsIsInterruptContext(void)
{
	uint32 CurrentPsr = 0;
	
	OsGetPSR(&CurrentPsr);
	
	return((CurrentPsr & EXCEPTION_CODE_MASK) == 0 ? FALSE : TRUE);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OsIsCat2IntContext
///
/// \descr  This function check if the cpu is executing a category 2 interrupt.
///
/// \param  void
///
/// \return boolean: TRUE -> Category 2 interrupt context, FALSE -> none category 2 interrupt context
//------------------------------------------------------------------------------------------------------------------
boolean OsIsCat2IntContext(void)
{
		return((boolean)OCB_Cfg.OsIsrInterruptLevel);
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_DisableAllInterrupts
///
/// \descr  Disable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_DisableAllInterrupts(void)
{
	DISABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_EnableAllInterrupts
///
/// \descr  enable all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_EnableAllInterrupts(void)
{
	ENABLE_INTERRUPTS();
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendAllInterrupts
///
/// \descr  Suspend all interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendAllInterrupts(void)
{
	DISABLE_INTERRUPTS();
}	

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeAllInterrupts
///
/// \descr  Resume all suspended interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeAllInterrupts(void)
{
	ENABLE_INTERRUPTS();	
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SuspendOSInterrupts
///
/// \descr  Suspend all OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_SuspendOSInterrupts(void)
{
	DISABLE_INTERRUPTS();	
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ResumeOSInterrupts
///
/// \descr  Resume all suspended OS interrupts.
///
/// \param  void
///
/// \return void
//------------------------------------------------------------------------------------------------------------------
void OS_ResumeOSInterrupts(void)
{
	ENABLE_INTERRUPTS();	
}


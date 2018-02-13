// *****************************************************************************
// Filename    : OsAPIs.h
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
// Description : Os APIs prototypes
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef __OSAPIS_H__
#define __OSAPIS_H__

#include"OsTypes.h"
#include"OsInternal.h"

OsStatusType OS_ActivateTask(OsTaskType TaskID);
OsStatusType OS_TerminateTask(void);
OsStatusType OS_ChainTask(OsTaskType TaskID);
OsStatusType OS_Schedule(void);
	
OsStatusType OS_GetTaskID(OsTaskRefType TaskID);
OsStatusType OS_GetTaskState(OsTaskType TaskID, OsTaskStateRefType State);


void OS_DisableAllInterrupts(void);
void OS_EnableAllInterrupts(void);
void OS_SuspendAllInterrupts(void);
void OS_ResumeAllInterrupts(void);
void OS_SuspendOSInterrupts(void);
void OS_ResumeOSInterrupts(void);



OsStatusType OS_GetResource(OsResourceType ResID);
OsStatusType OS_ReleaseResource(OsResourceType ResID);
OsStatusType OS_SetEvent(OsTaskType TaskID, OsEventMaskType Mask);
OsStatusType OS_ClearEvent(OsEventMaskType Mask);
OsStatusType OS_GetEvent(OsTaskType TaskID, OsEventMaskRefType Event);
OsStatusType OS_WaitEvent(OsEventMaskType Mask);


OsStatusType OS_GetAlarmBase(OsAlarmType AlarmID, OsAlarmBaseRefType Info);
OsStatusType OS_GetAlarm(OsAlarmType AlarmID, OsTickRefType Tick);
OsStatusType OS_SetRelAlarm(OsAlarmType AlarmID, OsTickType increment, OsTickType cycle);
OsStatusType OS_SetAbsAlarm(OsAlarmType AlarmID, OsTickType start, OsTickType cycle);
OsStatusType OS_CancelAlarm(OsAlarmType AlarmID);



OsAppModeType OS_GetActiveApplicationMode(void);
void OS_StartOS(OsAppModeType Mode);
void OS_ShutdownOS(OsStatusType Error);

extern void ErrorHook(OsStatusType Error);
extern void PreTaskHook(void);
extern void PostTaskHook(void);
extern void StartupHook(void);
extern void ShutdownHook(OsStatusType Error);

#endif


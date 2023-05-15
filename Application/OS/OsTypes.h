// *****************************************************************************
// Filename    : OsTypes.h
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
// Description : Os types definition header file
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************

#ifndef __OS_TYPES_H__
#define __OS_TYPES_H__

#include"types.h"
#include"HwPltfm.h"

typedef enum
{
  PRE_READY,
  READY,
  WAITING,
  RUNNING,
  SUSPENDED,
  INVALID_STATE
}OsTaskStateType;

typedef enum
{
  BASIC,
  EXTENDED
}OsTasksType;

typedef enum
{
  NONE_PREEMPT,
  FULL_PREEMPT
}OsTasksSchedType;

typedef enum
{
  E_OK                       = 0,
  E_OS_ACCESS                = 1,
  E_OS_CALLEVEL              = 2,
  E_OS_ID                    = 3,
  E_OS_LIMIT                 = 4,
  E_OS_NOFUNC                = 5,
  E_OS_RESOURCE              = 6,
  E_OS_STATE                 = 7,
  E_OS_VALUE                 = 8,
  E_OS_SERVICEID             = 9,
  E_OS_ILLEGAL_ADDRESS       = 10,
  E_OS_MISSINGEND            = 11,
  E_OS_DISABLEDINT           = 12,
  E_OS_STACKFAULT            = 13,
  E_OS_PROTECTION_MEMORY     = 14,
  E_OS_PROTECTION_TIME       = 15,
  E_OS_PROTECTION_ARRIVAL    = 16,
  E_OS_PROTECTION_LOCKED     = 17,
  E_OS_PROTECTION_EXCEPTION  = 18,
  E_OS_SYS_ASSERTION         = 20,
  E_OS_SYS_ABORT             = 21,
  E_OS_SYS_API_ERROR         = 23,
  E_OS_SYS_ALARM_MANAGEMENT  = 24,
  E_OS_SYS_WARNING           = 25
}OsStatusType;

typedef enum
{
  ALARM_SET_EVENT,
  ALARM_ACTIVE_TASK,
  ALARM_CALLBACK
  
}OsAlarmAction;

typedef enum
{
  ALARM_FREE,
  ALARM_BUSY
}AlarmStatus;

typedef enum
{
  ALARM_ONE_SHOT,
  ALARM_CYCLIC
}AlarmTypes;

typedef enum
{
  ALARM_RELATIVE,
  ALARM_ABSOLUTE
}AlarmCatgys;

/* OS TYPES */
typedef unsigned int OsTaskType;
typedef unsigned int OsEventMaskType;
typedef OsEventMaskType* OsEventMaskRefType;
typedef OsTaskType* OsTaskRefType; 
typedef OsTaskStateType* OsTaskStateRefType;
typedef unsigned int OsResourceType;
typedef unsigned int OsAppModeType;
typedef unsigned int OsAlarmType;
typedef unsigned int OsTickType;
typedef OsTickType* OsTickRefType;

/* TCB & CCB typedef */

typedef struct
{
  const pTaskFunc   function;
  const uint32      FixedPrio;
  const uint32      Autostart;
  const uint32      pstack_top;
  const uint32      pstack_bot;
  uint32            pCurrentStackPointer;
  OsTaskStateType   TaskStatus;
  uint32            Prio;  
  uint32            CeilingPrio;  
  uint32            SetEvtMask;
  uint32            WaitEvtMask;
  uint32            NbOfActiv;
  OsTasksType       TaskType;
  OsTasksSchedType  TaskSchedType;
}Tcb_t;

typedef struct
{
  OsTickType             InitTicks;
  OsTickType             InitCycles;
  const OsEventMaskType  Event;
  const OsTaskType       TaskId;
  const OsAlarmAction    Action;
  const uint32           AutoStart;
  AlarmStatus            Status;
  AlarmTypes             Alarmtype;
  AlarmCatgys            AlarmCategory;
  OsTickType             AlarmCheckPoint;
  pTaskFunc              CallBackFunc;
}Alarm_t;

typedef struct
{
  const uint32 ResCeilingPrio;
  uint32 CurrentOccupiedTask;
  const uint32* AuthorizedTask;
}Resource_t;

typedef Alarm_t** OsAlarmBaseRefType;

typedef struct
{
  Tcb_t**       pTcb;
  Alarm_t**     pAlarm;
  Resource_t**  pRes;
  uint32        HighPrioReadyTaskIdx;
  uint32        CurrentTaskIdx;
  uint64        OsSysTickCounter;
  uint32        OsIsrCallDispatcher;
  uint32        OsLockDispatcher;
  uint32        OsIsrInterruptLevel;
  uint32        OsSystemStackPtr;
  uint32        OsCurrentSystemStackPtr;
}Ocb_t;

typedef struct
{
  uint32 IsrNum;
  pFunc  IsrFunc;
}Isr_t;

#define TASK(x) void Task##x##Func(void)
#define ISR(x)  void Os##x##Isr(void)
#define pTASK(x) &Task##x##Func
#define STACK(x,y) uint32 Stack_T_##x[(y/4)]; const uint32 Stack_T_##x##_Size = y
#define TSTACK(x) (uint32)(&Stack_T_##x[((Stack_T_##x##_Size)/4)-1])
#define BSTACK(x) (uint32)(&Stack_T_##x[0])

#define OS_DeclareTask(TaskId)
#define OS_DeclareResource(ResId)
#define OS_DeclareEvent(Event)
#define OS_DeclareAlarm(AlarmId)

#define OS_AUTOSTART    (uint32)1
#define OS_NO_AUTOSTART (uint32)0

#define ALARM_AUTOSTART    (uint32)1
#define ALARM_NO_AUTOSTART (uint32)0

#define APP_MODE_DEFAULT (OsAppModeType)0
#define APP_MODE APP_MODE_DEFAULT

#endif

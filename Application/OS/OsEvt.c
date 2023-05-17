// *****************************************************************************************************************
// Filename    : OsEvent.c
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
// Description : Event & resource mechanism implementation
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************************************************

//------------------------------------------------------------------------------------------------------------------
// Include files
//------------------------------------------------------------------------------------------------------------------
#include"TCB.h"
#include"OsAPIs.h"

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetResource
///
/// \descr  This call serves to enter critical sections in the code that are assigned to the resource referenced 
///         by <ResID>. A critical section shall always be left using ReleaseResource
///
/// \param  OsResourceType ResID: Reference to resource
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetResource(OsResourceType ResID)
{
  if(ResID < NB_OF_RESOURCE)
  {
    if(OCB_Cfg.pRes[ResID]->AuthorizedTask[OCB_Cfg.CurrentTaskIdx] == 1 &&
       OCB_Cfg.pRes[ResID]->CurrentOccupiedTask == INVALID_TASK)
    {
      /* The resource is available */

      /* reserve the resource to the current task */
      OCB_Cfg.pRes[ResID]->CurrentOccupiedTask = OCB_Cfg.CurrentTaskIdx;

      /* Set the ceilling prio of the resource to the current task */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->CeilingPrio = OCB_Cfg.pRes[ResID]->ResCeilingPrio;
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->CeilingPrio;

      return(E_OK);
    }
    else
    {
      /* Resource is occupied by another task */
      return(E_OS_ACCESS);
    }
  }
  else
  {
    return(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ReleaseResource
///
/// \descr  ReleaseResource is the counterpart of GetResource and serves to leave critical sections in the code 
///         that are assigned to the resource referenced by <ResID>
///
/// \param  OsResourceType ResID: Reference to resource
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_ReleaseResource(OsResourceType ResID)
{
  if(ResID < NB_OF_RESOURCE)
  {
    if(OCB_Cfg.pRes[ResID]->CurrentOccupiedTask == OCB_Cfg.CurrentTaskIdx)
    {
      /* Release the resource */
      OCB_Cfg.pRes[ResID]->CurrentOccupiedTask = INVALID_TASK;

      /* Set the default prio to the current task */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->CeilingPrio = 0;
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio = OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->FixedPrio;

      if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
      {
        if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskSchedType == FULL_PREEMPT)
        {
          /* Call the scheduler */
          (void)OS_Schedule();
        }
      }
      else
      {
        /* No active task, system in Idle state (OS_IdleLoop) */
        /* Call the scheduler */
        (void)OS_Schedule();
      }

      return(E_OK);
    }
    else
    {
      return(E_OS_NOFUNC);
    }
  }
  else
  {
    return(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SetEvent
///
/// \descr  The events of task <TaskID> are set according to the event mask <Mask>. 
///         Calling SetEvent causes the task <TaskID> to be transferred to the ready state, 
///         if it was waiting for at least one of the events specified in <Mask>.
///
/// \param  OsTaskType TaskID    : Reference to the task for which one or several events are to be set
///         OsEventMaskType Mask : Mask of the events to be set
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_SetEvent(OsTaskType TaskID, OsEventMaskType Mask)
{
  if(OCB_Cfg.pTcb[TaskID]->TaskType == BASIC)
  {
    return(E_OS_ACCESS);
  }
  else if(TaskID >= NB_OF_TASKS)
  {
    return(E_OS_ID);
  }
  else if(OCB_Cfg.pTcb[TaskID]->TaskStatus == SUSPENDED)
  {
    return(E_OS_STATE);
  }
  else
  {
    OCB_Cfg.pTcb[TaskID]->SetEvtMask |= Mask;

    if(OCB_Cfg.pTcb[TaskID]->TaskStatus == WAITING)
    {
      if((OCB_Cfg.pTcb[TaskID]->SetEvtMask & OCB_Cfg.pTcb[TaskID]->WaitEvtMask) != 0)
      {
        /* Switch state to Ready */
        OCB_Cfg.pTcb[TaskID]->TaskStatus = READY;

        if(OCB_Cfg.CurrentTaskIdx < NB_OF_TASKS)
        {
          if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskSchedType == FULL_PREEMPT)
          {
            /* Call the scheduler */
            (void)OS_Schedule();
          }
        }
        else
        {
          /* No active task, system in Idle state (OS_IdleLoop) */
          /* Call the scheduler */
          (void)OS_Schedule();
        }
      }
    }

    return(E_OK);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_ClearEvent
///
/// \descr  The events of the extended task calling ClearEvent are cleared according to the event mask <Mask>. 
///
/// \param  OsEventMaskType Mask : Mask of the events to be clear
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_ClearEvent(OsEventMaskType Mask)
{
  if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskType == BASIC)
  {
    return(E_OS_ACCESS);
  }
  else if(TRUE == OsIsInterruptContext())
  {
    return(E_OS_CALLEVEL);
  }
  else
  {
    OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->SetEvtMask &=(OsEventMaskType)(~Mask);

    return(E_OK);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetEvent
///
/// \descr  This service returns the current state of all event bits of the task <TaskID>, 
///         not the events that the task is waiting for.
///
/// \param         OsTaskType TaskID (in) : Task whose event mask is to be returned
///         OsEventMaskRefType Event (out): Reference to the memory of the return data
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetEvent(OsTaskType TaskID, OsEventMaskRefType Event)
{
  if(OCB_Cfg.pTcb[TaskID]->TaskType == BASIC)
  {
    return(E_OS_ACCESS);
  }
  else if(TaskID >= NB_OF_TASKS)
  {
    return(E_OS_ID);
  }
  else if(OCB_Cfg.pTcb[TaskID]->TaskStatus == SUSPENDED)
  {
    return(E_OS_STATE);
  }
  else
  {
    *Event = OCB_Cfg.pTcb[TaskID]->SetEvtMask;
    return(E_OK);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_WaitEvent
///
/// \descr  The state of the calling task is set to waiting, unless at least one of the events specified in <Mask> 
///         has already been set. 
///
/// \param  OsEventMaskType Mask : Mask of the events waited for
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_WaitEvent(OsEventMaskType Mask)
{
  if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->CeilingPrio != 0 || OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->Prio != OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->FixedPrio)
  {
  #if(ERRORHOOK)
    ErrorHook(E_OS_RESOURCE);
  #endif
    return(E_OS_RESOURCE);
  }
  else if(OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskType == BASIC)
  {
  #if(ERRORHOOK)
    ErrorHook(E_OS_ACCESS);
  #endif
    return(E_OS_ACCESS);
  }
  else if(TRUE == OsIsInterruptContext())
  {
  #if(ERRORHOOK)
    ErrorHook(E_OS_CALLEVEL);
  #endif
    return(E_OS_CALLEVEL);
  }
  else
  {
    /* Store the new event mask*/
    OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->WaitEvtMask = Mask;

    /* Check if the event waiting for is already set */
    if((OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->SetEvtMask & OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->WaitEvtMask) == 0)
    {
      /* event not present -> set current task to waiting */
      OCB_Cfg.pTcb[OCB_Cfg.CurrentTaskIdx]->TaskStatus = WAITING;

      /* Call the scheduler */
      (void)OS_Schedule();
    }

    return(E_OK);
  }
}

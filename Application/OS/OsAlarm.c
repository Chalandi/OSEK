// *****************************************************************************************************************
// Filename    : OsAlarm.c
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
// Description : Alarm management implementation
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
/// \brief  OS_GetAlarmBase
///
/// \descr  The system service GetAlarmBase reads the alarm base characteristics.
///
/// \param      OsAlarmType AlarmID (In) : Reference to alarm
///         OsAlarmBaseRefType Info (Out): Reference to structure with constants of the alarm base
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetAlarmBase(OsAlarmType AlarmID, OsAlarmBaseRefType Info)
{
  if(AlarmID < NB_OF_ALARMS)
  {
    Info = &OCB_Cfg.pAlarm[AlarmID];

    return(E_OK);
  }
  else
  {
    (void)Info;

    return(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_GetAlarm
///
/// \descr  The system service GetAlarm returns the relative value in ticks before the alarm <AlarmID> expires
///
/// \param   OsAlarmType AlarmID (In) : Reference to alarm
///          OsTickRefType Tick (Out) : Relative value in ticks before the alarm <AlarmID> expires
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_GetAlarm(OsAlarmType AlarmID, OsTickRefType Tick)
{
  if(AlarmID < NB_OF_ALARMS)
  {
    *Tick = OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint - (uint32)OCB_Cfg.OsSysTickCounter;

    return(E_OK);
  }
  else
  {
    return(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SetRelAlarm
///
/// \descr  The system service occupies the alarm <AlarmID> element. After <increment> ticks have elapsed, 
///         the task assigned to the alarm <AlarmID> is activated or the assigned event (only for extended tasks) 
///         is set or the alarm-callback routine is called.
///
/// \param   OsAlarmType AlarmID  : Reference to the alarm element
///          OsTickType increment : Relative value in ticks
///          OsTickType cycle     : Cycle value in case of cyclic alarm. In case of single alarms, it shall be zero.
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_SetRelAlarm(OsAlarmType AlarmID, OsTickType increment, OsTickType cycle)
{
  if(AlarmID < NB_OF_ALARMS)
  {
    if(cycle == 0 && increment > 0 && OCB_Cfg.pAlarm[AlarmID]->Status == ALARM_FREE)
    {
      /* One shot alarm */
      OCB_Cfg.pAlarm[AlarmID]->Alarmtype       = ALARM_ONE_SHOT;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCategory   = ALARM_RELATIVE;
      OCB_Cfg.pAlarm[AlarmID]->InitCycles      = 0;
      OCB_Cfg.pAlarm[AlarmID]->InitTicks       = increment;
      OCB_Cfg.pAlarm[AlarmID]->Status          = ALARM_BUSY;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = increment + (uint32)OCB_Cfg.OsSysTickCounter;
      return(E_OK);
    }
    else if (cycle != 0 &&  cycle >= increment && OCB_Cfg.pAlarm[AlarmID]->Status == ALARM_FREE)
    {
      /* Cyclic alarm */
      OCB_Cfg.pAlarm[AlarmID]->Alarmtype       = ALARM_CYCLIC;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCategory   = ALARM_RELATIVE;      
      OCB_Cfg.pAlarm[AlarmID]->InitCycles      = cycle;
      OCB_Cfg.pAlarm[AlarmID]->InitTicks       = increment;
      OCB_Cfg.pAlarm[AlarmID]->Status          = ALARM_BUSY;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = increment + cycle + (uint32)OCB_Cfg.OsSysTickCounter;
      return(E_OK);
    }
    else if(OCB_Cfg.pAlarm[AlarmID]->Status == ALARM_BUSY)
    {
      return(E_OS_STATE);
    }
    else
    {
      return(E_OS_VALUE);
    }
  }
  else
  {
    return(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_SetAbsAlarm
///
/// \descr  The system service occupies the alarm <AlarmID> element. When <start> ticks are reached, 
///         the task assigned to the alarm <AlarmID> is activated or the assigned event (only for extended tasks) 
///         is set or the alarm-callback routine is called.
///
/// \param   OsAlarmType AlarmID  : Reference to the alarm element
///          OsTickType start     : Absolute value in ticks
///          OsTickType cycle     : Cycle value in case of cyclic alarm. In case of single alarms, it shall be zero.
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_SetAbsAlarm(OsAlarmType AlarmID, OsTickType start, OsTickType cycle)
{
  if(AlarmID < NB_OF_ALARMS)
  {
    if(cycle == 0 && start > (uint32)OCB_Cfg.OsSysTickCounter && OCB_Cfg.pAlarm[AlarmID]->Status == ALARM_FREE)
    {
      /* One shot alarm */
      OCB_Cfg.pAlarm[AlarmID]->Alarmtype       = ALARM_ONE_SHOT;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCategory   = ALARM_ABSOLUTE;
      OCB_Cfg.pAlarm[AlarmID]->InitCycles      = 0;
      OCB_Cfg.pAlarm[AlarmID]->InitTicks       = start;
      OCB_Cfg.pAlarm[AlarmID]->Status          = ALARM_BUSY;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = start;
      return(E_OK);
    }
    else if (cycle != 0 &&  start > (uint32)OCB_Cfg.OsSysTickCounter && OCB_Cfg.pAlarm[AlarmID]->Status == ALARM_FREE)
    {
      /* Cyclic alarm */
      OCB_Cfg.pAlarm[AlarmID]->Alarmtype       = ALARM_CYCLIC;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCategory   = ALARM_ABSOLUTE;
      OCB_Cfg.pAlarm[AlarmID]->InitCycles      = cycle;
      OCB_Cfg.pAlarm[AlarmID]->InitTicks       = start;
      OCB_Cfg.pAlarm[AlarmID]->Status          = ALARM_BUSY;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = start;
      return(E_OK);
    }
    else if(OCB_Cfg.pAlarm[AlarmID]->Status == ALARM_BUSY)
    {
      return(E_OS_STATE);
    }
    else
    {
      return(E_OS_VALUE);
    }
  }
  else
  {
    return(E_OS_ID);
  }
}

//------------------------------------------------------------------------------------------------------------------
/// \brief  OS_CancelAlarm
///
/// \descr  The system service cancels the alarm <AlarmID>
///
/// \param   OsAlarmType AlarmID : Reference to the alarm element
///
/// \return OsStatusType
//------------------------------------------------------------------------------------------------------------------
OsStatusType OS_CancelAlarm(OsAlarmType AlarmID)
{
  if(AlarmID < NB_OF_ALARMS)
  {
      OCB_Cfg.pAlarm[AlarmID]->Status          = ALARM_FREE;
      OCB_Cfg.pAlarm[AlarmID]->AlarmCheckPoint = 0;
      return(E_OK);
  }
  else
  {
    return(E_OS_ID);
  }
}

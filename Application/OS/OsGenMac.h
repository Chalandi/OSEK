// *****************************************************************************************************************
// Filename    : OsGenMac.h
// 
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 27.12.2017
// 
// Description : X Macros used to generate the TCB and OCB of the OS
//
// License     : GNU General Public License v3.0
//  
// *****************************************************************************************************************

#ifndef _OS_GEN_MAC_H_
#define _OS_GEN_MAC_H_


#endif

#include"OsTypes.h"

#ifdef OS_CONFIG_BEGIN
#undef OS_CONFIG_BEGIN
#endif

#ifdef OS_TASK_BEGIN
#undef OS_TASK_BEGIN
#endif

#ifdef OS_TASK_DEF
#undef OS_TASK_DEF
#endif

#ifdef OS_TASK_END
#undef OS_TASK_END
#endif

#ifdef OS_EVENT_BEGIN
#undef OS_EVENT_BEGIN
#endif

#ifdef OS_EVENT_DEF
#undef OS_EVENT_DEF
#endif

#ifdef OS_EVENT_END
#undef OS_EVENT_END
#endif

#ifdef OS_ALARM_BEGIN
#undef OS_ALARM_BEGIN
#endif

#ifdef OS_ALARM_DEF
#undef OS_ALARM_DEF
#endif

#ifdef OS_ALARM_END
#undef OS_ALARM_END
#endif

#ifdef OS_RESOURCE_BEGIN
#undef OS_RESOURCE_BEGIN
#endif

#ifdef OS_RESOURCE_DEF
#undef OS_RESOURCE_DEF
#endif

#ifdef OS_RESOURCE_END
#undef OS_RESOURCE_END
#endif

#ifdef OS_INTERRUPT_BEGIN
#undef OS_INTERRUPT_BEGIN
#endif 
        
#ifdef OS_INTERRUPT_CAT2_DEF
#undef OS_INTERRUPT_CAT2_DEF
#endif

#ifdef OS_INTERRUPT_END
#undef OS_INTERRUPT_END
#endif

#ifdef OS_CONFIG_END
#undef OS_CONFIG_END
#endif


#if defined(OS_GEN_ENUM)

#define OS_CONFIG_BEGIN

#define OS_TASK_BEGIN                                                            enum{
#define OS_TASK_DEF(Name,Prio,StackSize,NbOfActiv,AutoStart,TaskType,SchedType)  Name##,
#define OS_TASK_END                                                              INVALID_TASK};

#define NB_OF_TASKS INVALID_TASK

#define OS_EVENT_BEGIN                                                           enum{
#define OS_EVENT_DEF(Event, Mask)                                                Event## = Mask,
#define OS_EVENT_END                                                             INVALID_EVENT};

#define NB_OF_EVENTS INVALID_EVENT

#define OS_ALARM_BEGIN                                                           enum{
#define OS_ALARM_DEF(Name,Action,Event,AutoStart,task,Callback)                  Name##,
#define OS_ALARM_END                                                             INVALID_ALARM};

#define NB_OF_ALARMS INVALID_ALARM

#define OS_RESOURCE_BEGIN                                                        enum{
#define OS_RESOURCE_DEF(Name,CeilingPrio,AuthorizedTasks...)                     Name##,
#define OS_RESOURCE_END                                                          INVALID_RESOURCE};

#define NB_OF_RESOURCE INVALID_RESOURCE

#define OS_INTERRUPT_BEGIN                                                      enum{
#define OS_INTERRUPT_CAT2_DEF(IsrNbr,IsrFunc)                                   IsrFunc##,
#define OS_INTERRUPT_END                                                        INVALID_ISR};

#define NB_OF_ISR INVALID_ISR

#define OS_CONFIG_END

#elif defined(OS_GEN_FUNC_PROTO)

#define OS_CONFIG_BEGIN

#define OS_TASK_BEGIN                                                            
#define OS_TASK_DEF(Name,Prio,StackSize,NbOfActiv,AutoStart,TaskType,SchedType)  
#define OS_TASK_END                                                              


#define OS_EVENT_BEGIN                                                           
#define OS_EVENT_DEF(Event, Mask)                                                
#define OS_EVENT_END                                                             



#define OS_ALARM_BEGIN                                                           
#define OS_ALARM_DEF(Name,Action,Event,AutoStart,task,Callback)                  
#define OS_ALARM_END                                                             



#define OS_RESOURCE_BEGIN                                                        
#define OS_RESOURCE_DEF(Name,CeilingPrio,AuthorizedTasks...)                     
#define OS_RESOURCE_END                                                          


#define OS_INTERRUPT_BEGIN                                                      extern Isr_t IsrLookupTable[];
#define OS_INTERRUPT_CAT2_DEF(IsrNbr,IsrFunc)                                   extern void Os##IsrFunc##Isr(void);
#define OS_INTERRUPT_END                                                         



#define OS_CONFIG_END

#elif defined(OS_GEN_DATA_STRUCT)

#define OS_CONFIG_BEGIN

#define OS_TASK_BEGIN
#define OS_TASK_DEF(Name,Prio,StackSize,NbOfActiv,AutoStart,TaskType,SchedType)  TASK(Name); STACK(Name,StackSize); static Tcb_t Tcb_t_##Name = {pTASK(Name) ,Prio ,AutoStart ,TSTACK(Name) ,BSTACK(Name) ,TSTACK(Name) ,SUSPENDED ,0 ,0 ,0 ,0 ,NbOfActiv, TaskType, SchedType};
#define OS_TASK_END

#define OS_EVENT_BEGIN
#define OS_EVENT_DEF(Event, Mask)
#define OS_EVENT_END

#define OS_ALARM_BEGIN
#define OS_ALARM_DEF(Name,Action,Event,AutoStart,task,Callback) static Alarm_t Alarm_##Name = {0, 0,  Event,task, Action, AutoStart, ALARM_FREE, 0, 0, 0, Callback};
#define OS_ALARM_END

#define OS_RESOURCE_BEGIN
#define OS_RESOURCE_DEF(Name,CeilingPrio,AuthorizedTasks...) static uint32 AuthorizedTask_##Name[NB_OF_TASKS] = AuthorizedTasks; static Resource_t Resource_##Name = {CeilingPrio, INVALID_TASK, &AuthorizedTask_##Name[0]};
#define OS_RESOURCE_END

#define OS_INTERRUPT_BEGIN                                                       Isr_t IsrLookupTable[] = {
#define OS_INTERRUPT_CAT2_DEF(IsrNbr,IsrFunc)                                    {IsrNbr,&Os##IsrFunc##Isr},
#define OS_INTERRUPT_END                                                         {0,0}};

#define OS_CONFIG_END

#elif defined(OS_GEN_TCB)

#define OS_CONFIG_BEGIN

#define OS_TASK_BEGIN                                                               static Tcb_t* CCB_Cfg[NB_OF_TASKS + 1] = {
#define OS_TASK_DEF(Name,Prio,StackSize,NbOfActiv,AutoStart,TaskType,SchedType)     &Tcb_t_##Name,
#define OS_TASK_END                                                                 0};

#define OS_EVENT_BEGIN
#define OS_EVENT_DEF(Event, Mask)
#define OS_EVENT_END

#define OS_ALARM_BEGIN                                                              static Alarm_t* Alarm_Cfg[NB_OF_ALARMS + 1]={
#define OS_ALARM_DEF(Name,Action,Event,AutoStart,task,Callback)                     &Alarm_##Name,
#define OS_ALARM_END                                                                0};

#define OS_RESOURCE_BEGIN                                                           static Resource_t* Res_Cfg[NB_OF_RESOURCE + 1]={
#define OS_RESOURCE_DEF(Name,CeilingPrio,AuthorizedTasks...)                        &Resource_##Name,
#define OS_RESOURCE_END                                                             0};

#define OS_INTERRUPT_BEGIN
#define OS_INTERRUPT_CAT2_DEF(IsrNbr,IsrFunc)                                      
#define OS_INTERRUPT_END

#define OS_CONFIG_END

#elif defined(OS_GEN_OCB)

#define OS_CONFIG_BEGIN                                                           volatile Ocb_t OCB_Cfg = { 

#define OS_TASK_BEGIN                                                             &CCB_Cfg[0],
#define OS_TASK_DEF(Name,Prio,StackSize,NbOfActiv,AutoStart,TaskType,SchedType)  
#define OS_TASK_END

#define OS_EVENT_BEGIN
#define OS_EVENT_DEF(Event, Mask)
#define OS_EVENT_END

#define OS_ALARM_BEGIN                                                            &Alarm_Cfg[0],
#define OS_ALARM_DEF(Name,Action,Event,AutoStart,task,Callback)                  
#define OS_ALARM_END

#define OS_RESOURCE_BEGIN                                                         &Res_Cfg[0],
#define OS_RESOURCE_DEF(Name,CeilingPrio,AuthorizedTasks...)                     
#define OS_RESOURCE_END

#define OS_INTERRUPT_BEGIN
#define OS_INTERRUPT_CAT2_DEF(IsrNbr,IsrFunc)                                    
#define OS_INTERRUPT_END

#define OS_CONFIG_END                                                             0,0,0,0,0,0,0,0};

#else
#error "OS generation error: Wrong way to include OsGenMac.h"
#endif


#ifdef OS_GEN_ENUM
#undef OS_GEN_ENUM
#endif

#ifdef OS_GEN_FUNC_PROTO
#undef OS_GEN_FUNC_PROTO
#endif

#ifdef OS_GEN_DATA_STRUCT
#undef OS_GEN_DATA_STRUCT
#endif

#ifdef OS_GEN_TCB
#undef OS_GEN_TCB
#endif

#ifdef OS_GEN_OCB
#undef OS_GEN_OCB
#endif




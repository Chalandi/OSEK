// *****************************************************************************
// Filename    : Appli.c
// 
// Core        : STM32F100RB Cortex™-M3  
// 
// Board       : STM32VLDISCOVERY
//
// Compiler    : ARM® Compiler v5.06 for μVision (Keil)
//
// Author      : Chalandi Amine
//
// Owner       : Chalandi Amine
// 
// Date        : 04.09.2017
// 
// Description : Main function
//
// License     : GNU General Public License v3.0
// 
// *****************************************************************************
#include"GPIO.h"
#include"TCB.h"
#include"OsAPIs.h"

static void Appli_LedInit(void);


#define TURN_ON_LED_BLUE() 	  GPIOC_ODR.ODR8 = 1
#define TURN_OFF_LED_BLUE() 	GPIOC_ODR.ODR8 = 0
#define TURN_ON_LED_GREEN() 	GPIOC_ODR.ODR9 = 1
#define TURN_OFF_LED_GREEN() 	GPIOC_ODR.ODR9 = 0

#define TOGGLE_BLUE_LED() 	  GPIOC_ODR.ODR8 ^= 1
#define TOGGLE_GREEN_LED() 	  GPIOC_ODR.ODR9 ^= 1

int main(void)
{
	Appli_LedInit();	
	OS_StartOS(APP_MODE_DEFAULT);
	return(0);
}

static void Appli_LedInit(void)
{  
	/* Set PC8 and PC9 output mode */	
	GPIOC_CRH.CN8   = 0;
	GPIOC_CRH.MODE8 = 3;
	GPIOC_CRH.CN9   = 0;
	GPIOC_CRH.MODE9 = 3;
}


TASK(T1)
{
	uint32 Cpt=0;
	OsEventMaskType OsWaitEventMask = EVT_BLINK_BLUE_LED;
	OsEventMaskType Events = 0;
	(void)OS_SetRelAlarm(ALARM_BLUE_LED,0,1000);

	for(;;)
	{
		Cpt++;
		if(E_OK == OS_WaitEvent(OsWaitEventMask))
		{	
			(void)OS_GetEvent((OsTaskType)T1, &Events);
			if((Events & EVT_BLINK_BLUE_LED) == EVT_BLINK_BLUE_LED)
			{
				OS_ClearEvent(EVT_BLINK_BLUE_LED);
				TOGGLE_BLUE_LED();
			}
			if(Cpt == 10)
			{
				OS_CancelAlarm(ALARM_BLUE_LED);
				OS_ChainTask(T2);			
			}
		}
		else
		{
			OS_TerminateTask(); /* In case of error we switch off the task */
		}
	}
}
#if 0
TASK(Idle)
{
	OsEventMaskType OsWaitEventMask = EVT_BLINK_GREEN_LED;
	OsEventMaskType Events = 0;
	(void)OS_SetRelAlarm(ALARM_GREEN_LED,0,1000);

	for(;;)
	{
		if(E_OK == OS_WaitEvent(OsWaitEventMask))
		{	
			(void)OS_GetEvent((OsTaskType)Idle, &Events);
			if((Events & EVT_BLINK_GREEN_LED) == EVT_BLINK_GREEN_LED)
			{
				OS_ClearEvent(EVT_BLINK_GREEN_LED);
				TOGGLE_GREEN_LED();
			}
		}
		else
		{
			OS_TerminateTask(); /* In case of error we switch off the task */
		}
	}
}
#else
TASK(Idle)
{
	OsEventMaskType Events = 0;
	(void)OS_SetRelAlarm(ALARM_GREEN_LED,0,1000);

	for(;;)
	{
		if(1)
		{	(void)OS_GetResource(RES_TRACE);
			(void)OS_GetEvent((OsTaskType)Idle, &Events);
			if((Events & EVT_BLINK_GREEN_LED) == EVT_BLINK_GREEN_LED)
			{
				OS_ClearEvent(EVT_BLINK_GREEN_LED);
				TOGGLE_GREEN_LED();
			}
			(void)OS_ReleaseResource(RES_TRACE);
		}
		else
		{
			OS_TerminateTask(); /* In case of error we switch off the task */
		}
	}
}
#endif
TASK(T2)
{
	OsEventMaskType OsWaitEventMask = EVT_BLINK_FASTER_BLUE_LED;
	OsEventMaskType Events = 0;
	(void)OS_SetRelAlarm(ALARM_FASTER_BLUE_LED,0,50);

	for(;;)
	{
		(void)OS_WaitEvent(OsWaitEventMask);
		(void)OS_GetEvent((OsTaskType)T2, &Events);
		if((Events & EVT_BLINK_FASTER_BLUE_LED) == EVT_BLINK_FASTER_BLUE_LED)
		{
			OS_ClearEvent(EVT_BLINK_FASTER_BLUE_LED);
			TOGGLE_BLUE_LED();
		}	
	}
}

TASK(T3)
{
	for(;;);
}

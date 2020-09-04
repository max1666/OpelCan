/*-- File description -------------------------------------------------------*/
/**
*   @file:    SWtimer.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef _SWTIMER_H
#define _SWTIMER_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef struct
{
	uint32_t Parameter1;
	uint32_t Parameter2;
}SWtimerParam_t;

typedef enum
{
	SWTT_None,
	SWTT_One_low,
	SWTT_Multi_low,
	SWTT_One_high,
	SWTT_Multi_high,
}SWtimerType_t;

typedef void (*SWtimerCallback)(SWtimerParam_t);

typedef struct
{
	SWtimerType_t 		SWtimerType;
	uint32_t            StartDelay;
	uint32_t 			Period;
	uint32_t 			Ticks;
	SWtimerCallback 	Callback;
	SWtimerParam_t 		Parameters;
}SWtimer_Callback_t;

typedef struct
{
	SWtimerCallback 	Callback;
	SWtimerParam_t 		Parameters;
}SWtimer_LowPriority_t;

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
bool SWtimer_IsTimerCallback(uint32_t id);
uint32_t SWtimer_RunTimerCallback(SWtimerType_t timerType, uint32_t startDelay, uint32_t period, SWtimerCallback callback, SWtimerParam_t *parameters);
bool SWtimer_RefreshTimerCallback(uint32_t id, uint32_t period);
bool SWtimer_PauseTimerCallback(uint32_t id, uint32_t delay);
bool SWtimer_StopTimerCallback(uint32_t id);

bool SWtimer_IsTimerExternal(uint32_t *ust);
bool SWtimer_RunTimerExternal(uint32_t *ust);
bool SWtimer_ResetTimerExternal(uint32_t *ust);

uint32_t *SWtimer_RunTimerInternal(uint32_t ust);
bool SWtimer_ResetTimerInternal(uint32_t *ust);

void SWtimer_Delay(uint32_t ms);


void SWtimer_TimerTick_HighPriority(uint32_t msPerTick);
void SWtimer_TimerTick_LowPriority(void);

#endif // _SWTIMER_H
/*-- EOF --------------------------------------------------------------------*/

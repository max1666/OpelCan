/*-- File description -------------------------------------------------------*/
/**
*   @file:    SWtimer.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "SWtimer.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define SW_TIMER__COUNT_HIGH_EXTERNAL	5
#define SW_TIMER__COUNT_HIGH_INTERNAL	5
#define SW_TIMER__COUNT_HIGH_CALLBACK	10
#define SW_TIMER__COUNT_LOW_CALLBACK	10

/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint32_t *swTimer_ext[SW_TIMER__COUNT_HIGH_EXTERNAL] = {0};
static uint32_t swTimer_int[SW_TIMER__COUNT_HIGH_INTERNAL + 1] = {0};
static SWtimer_Callback_t swTimer_callbacks_high[SW_TIMER__COUNT_HIGH_CALLBACK];
static SWtimer_LowPriority_t swTimer_callbacks_low[SW_TIMER__COUNT_LOW_CALLBACK];

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
bool SWtimer_IsTimerCallback(uint32_t id)
{
	bool result = false;

	uint8_t i = id - 1;
	if(swTimer_callbacks_high[i].SWtimerType != SWTT_None)
	{
		result = true;
	}
	
	return result;
}

uint32_t SWtimer_RunTimerCallback(SWtimerType_t timerType, uint32_t startDelay, uint32_t period, SWtimerCallback callback, SWtimerParam_t *parameters)
{
	uint32_t result = 0;

	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_HIGH_CALLBACK; i++)
	{
		if(swTimer_callbacks_high[i].SWtimerType == SWTT_None)
		{
			swTimer_callbacks_high[i].Callback = callback;
			swTimer_callbacks_high[i].StartDelay = startDelay;
			swTimer_callbacks_high[i].Period = period;
			swTimer_callbacks_high[i].Ticks = period;
			swTimer_callbacks_high[i].SWtimerType = timerType;

			if(parameters)
			{
				memcpy(&swTimer_callbacks_high[i].Parameters, parameters, sizeof(SWtimerParam_t));
			}

			result = i + 1;
			break;
		}
	}

	return result;
}

bool SWtimer_RefreshTimerCallback(uint32_t id, uint32_t period)
{
	bool result = false;

	if((id > 0) && (id < SW_TIMER__COUNT_HIGH_CALLBACK + 1))
	{
		uint8_t i = id - 1;
		if(swTimer_callbacks_high[i].SWtimerType != SWTT_None)
		{
			swTimer_callbacks_high[i].Period = period;
			swTimer_callbacks_high[i].Ticks = period;

			result = true;
		}
	}
	return result;
}

bool SWtimer_PauseTimerCallback(uint32_t id, uint32_t delay)
{
	bool result = false;

	if((id > 0) && (id < SW_TIMER__COUNT_HIGH_CALLBACK + 1))
	{
		uint8_t i = id - 1;
		if(swTimer_callbacks_high[i].SWtimerType != SWTT_None)
		{
			swTimer_callbacks_high[i].StartDelay = delay;

			result = true;
		}
	}
	return result;
}

bool SWtimer_StopTimerCallback(uint32_t id)
{
	bool result = false;

	if((id > 0) && (id < SW_TIMER__COUNT_HIGH_CALLBACK + 1))
	{
		uint8_t i = id - 1;
		if(swTimer_callbacks_high[i].SWtimerType != SWTT_None)
		{
			swTimer_callbacks_high[i].SWtimerType = SWTT_None;
			swTimer_callbacks_high[i].Callback = NULL;
			memset(&swTimer_callbacks_high[i].Parameters, 0x00, sizeof(SWtimerParam_t));
			swTimer_callbacks_high[i].StartDelay = 0;
			swTimer_callbacks_high[i].Period = 0;
			swTimer_callbacks_high[i].Ticks = 0;

			result = true;
		}
	}
	
	return result;
}

//---------------------------------------------------------------------------

uint32_t *SWtimer_RunTimerInternal(uint32_t ust)
{
	uint32_t *result = NULL;

	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_HIGH_INTERNAL; i++)
	{
		if(!swTimer_int[i])
		{
			swTimer_int[i] = ust;
			
			result = &swTimer_int[i];
			break;
		}
	}

	return result;
}

bool SWtimer_ResetTimerInternal(uint32_t *ust)
{
	bool result = false;

	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_HIGH_INTERNAL; i++)
	{
		if(&swTimer_int[i] == ust)
		{
			swTimer_int[i] = 0;
			
			result = true;
			break;
		}
	}
	
	return result;
}

//---------------------------------------------------------------------------

bool SWtimer_IsTimerExternal(uint32_t *ust)
{
	bool result = false;

	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_HIGH_EXTERNAL; i++)
	{
		if(swTimer_ext[i] == ust)
		{
			result = true;
			break;
		}
	}
	
	return result;
}

bool SWtimer_RunTimerExternal(uint32_t *ust)
{
	bool result = false;

	if(SWtimer_IsTimerExternal(ust))
	{
		result = true;
	}
	else
	{
		uint8_t i = 0;
		for(i = 0; i < SW_TIMER__COUNT_HIGH_EXTERNAL; i++)
		{
			if(!swTimer_ext[i])
			{
				swTimer_ext[i] = ust;
				
				result = true;
				break;
			}
		}
	}
	
	return result;
}

bool SWtimer_ResetTimerExternal(uint32_t *ust)
{
	bool result = false;

	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_HIGH_EXTERNAL; i++)
	{
		if(swTimer_ext[i] == ust)
		{
			swTimer_ext[i] = NULL;
			
			result = true;
			break;
		}
	}
	
	return result;
}

//---------------------------------------------------------------------------

bool SWtimer_LowTimer_AddCycle(SWtimerCallback callback, SWtimerParam_t *parameters)
{
	bool result = true;

	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_LOW_CALLBACK; i++)
	{
		if(!swTimer_callbacks_low[i].Callback)
		{
			swTimer_callbacks_low[i].Callback = callback;
			memcpy(&swTimer_callbacks_low[i].Parameters, parameters, sizeof(SWtimerParam_t));

			result = true;
			break;
		}
	}

	return result;
}

//---------------------------------------------------------------------------

void SWtimer_Delay(uint32_t ms)
{
	swTimer_int[SW_TIMER__COUNT_HIGH_INTERNAL] = ms;

	while(swTimer_int[SW_TIMER__COUNT_HIGH_INTERNAL] > 0)
	{
		continue;
	}
}

//---------------------------------------------------------------------------

void SWtimer_TimerTick_HighPriority(uint32_t msPerTick)
{
	uint8_t i = 0;

	for(i = 0; i < SW_TIMER__COUNT_HIGH_EXTERNAL; i++)
	{
		if(swTimer_ext[i])
		{
			if((*swTimer_ext[i]) > msPerTick)
			{
				(*swTimer_ext[i]) -= msPerTick;
			}
			else
			{
				*swTimer_ext[i] = 0;
				swTimer_ext[i] = NULL;
			}
		}			
	}
	
	for(i = 0; i < SW_TIMER__COUNT_HIGH_INTERNAL + 1; i++)
	{
		if(swTimer_int[i] > 0)
		{
			if(swTimer_int[i] > msPerTick)
			{
				swTimer_int[i] -= msPerTick;
			}
			else
			{
				swTimer_int[i] = 0;
			}
		}
	}
	
	for(i = 0; i < SW_TIMER__COUNT_HIGH_CALLBACK; i++)
	{
		if(swTimer_callbacks_high[i].SWtimerType == SWTT_None)
		{
			continue;
		}
		
		if(swTimer_callbacks_high[i].StartDelay > msPerTick)
		{
			swTimer_callbacks_high[i].StartDelay -= msPerTick;
		}
		else if (swTimer_callbacks_high[i].StartDelay > 0)
		{
			swTimer_callbacks_high[i].StartDelay = 0;
		}
		else
		{
			if (swTimer_callbacks_high[i].Ticks > msPerTick)
			{
				swTimer_callbacks_high[i].Ticks -= msPerTick;
			}
			else
			{
				swTimer_callbacks_high[i].Ticks = 0;
			}
		}
	
		//Р�РЅР°С‡Рµ
		if(swTimer_callbacks_high[i].Ticks == 0)
		{
			switch(swTimer_callbacks_high[i].SWtimerType)
			{
				case SWTT_None:
					break;
				
				case SWTT_One_low:
				{
					if(swTimer_callbacks_high[i].Callback)
					{
						SWtimer_LowTimer_AddCycle(swTimer_callbacks_high[i].Callback, &swTimer_callbacks_high[i].Parameters);
					}
					swTimer_callbacks_high[i].SWtimerType = SWTT_None;
				}
				break;
				
				case SWTT_Multi_low:
				{
					if(swTimer_callbacks_high[i].Callback)
					{
						SWtimer_LowTimer_AddCycle(swTimer_callbacks_high[i].Callback, &swTimer_callbacks_high[i].Parameters);
					}
					swTimer_callbacks_high[i].Ticks = swTimer_callbacks_high[i].Period;
				}
				break;
				
				case SWTT_One_high:
				{
					if(swTimer_callbacks_high[i].Callback)
					{
						swTimer_callbacks_high[i].Callback(swTimer_callbacks_high[i].Parameters);
					}
					swTimer_callbacks_high[i].SWtimerType = SWTT_None;
				}
				break;
				
				case SWTT_Multi_high:
				{
					if(swTimer_callbacks_high[i].Callback)
					{
						swTimer_callbacks_high[i].Callback(swTimer_callbacks_high[i].Parameters);
					}
					swTimer_callbacks_high[i].Ticks = swTimer_callbacks_high[i].Period;
				}
				break;
				 
			}
		}
		
	}
}

//---------------------------------------------------------------------------

void SWtimer_TimerTick_LowPriority(void)
{
	uint8_t i = 0;
	for(i = 0; i < SW_TIMER__COUNT_LOW_CALLBACK; i++)
	{
		if(swTimer_callbacks_low[i].Callback)
		{
			swTimer_callbacks_low[i].Callback(swTimer_callbacks_low[i].Parameters);
			swTimer_callbacks_low[i].Callback = NULL;
			memset(&swTimer_callbacks_low[i].Parameters, 0x00, sizeof(SWtimerParam_t));
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

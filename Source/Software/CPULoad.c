/*-- File description -------------------------------------------------------*/
/**
*   @file:    CPULoad.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "CPULoad.h"
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint32_t cpuIdleTicksCount = 0;
static uint32_t cpuTicks_ms = 0;
static uint32_t cpuTicks_Main = 0;
static float cpuLoad = 0.0;

static bool initInProcess = false;

/*-- Local functions --------------------------------------------------------*/
static void CPULoad_Calculate(void)
{
	uint32_t procTicks = cpuIdleTicksCount - cpuTicks_Main;
	
	cpuLoad = (procTicks * 100.0) / cpuIdleTicksCount;
}

/*-- Exported functions -----------------------------------------------------*/
void CPULoad_Init(void)
{
	initInProcess = true;

	cpuTicks_Main = 0;
	cpuTicks_ms = 0;
}

float CPULoad_Read(void)
{
	return cpuLoad;
}

void CPULoad_Tick_ms(uint32_t ms)
{
	cpuTicks_ms += ms;
	
	if(cpuTicks_ms >= 500)
	{
		if(initInProcess)
		{
			cpuIdleTicksCount = cpuTicks_Main;
			initInProcess = false;
		}
		else
		{
			CPULoad_Calculate();
		}
		
		cpuTicks_ms = 0;
		cpuTicks_Main = 0;
	}
}

void CPULoad_Tick_Main(void)
{
	cpuTicks_Main++;
}

/*-- EOF --------------------------------------------------------------------*/

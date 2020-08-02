/*-- File description -------------------------------------------------------*/
/**
*   @file:    stm32f10x_it.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "stm32f10x_it.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void NMI_Handler(void)
{
	
}

void HardFault_Handler(void)
{
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	while (1)
	{
	}
}

void SVC_Handler(void)
{
	
}

void DebugMon_Handler(void)
{
	
}

void PendSV_Handler(void)
{
	
}

void SysTick_Handler(void)
{
	
}

/*-- EOF --------------------------------------------------------------------*/

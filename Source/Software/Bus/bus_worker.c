/*-- File description -------------------------------------------------------*/
/**
*   @file:    bus_worker.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "bus_worker.h"
/*-- Standart C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "CAN1.h"

/*-- Project specific includes ----------------------------------------------*/
#include "CanHacker.h"
#include "HS_Can.h"
#include "MS_Can.h"
#include "LS_Can.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void CanBus_ReceiveMessage(CanRxMsg *message)
{
	
#if defined (USE_CAN_HACKER)
	CanHacker_ReceiveCanMessage(message);
#endif
	
	switch (CAN1_GetPrescaller())
	{
		case CAN1_HS_SPEED_PRESCALE:
		{
			HS_CAN__ReceiveMessage(message);
		}
		break;
		
		case CAN1_MS_SPEED_PRESCALE:
		{
			MS_CAN__ReceiveMessage(message);
		}
		break;
		
		case CAN1_LS_SPEED_PRESCALE:
		{
			LS_CAN__ReceiveMessage(message);
		}
		break;
		
		default:
		{
		}
		break;
	}
	
}

void CanBus_Run(void)
{
	
#if defined (USE_CAN_HACKER)
	CanHacker_Run();
#endif
	
	switch (CAN1_GetPrescaller())
	{
		case CAN1_HS_SPEED_PRESCALE:
		{
			HS_CAN__Run();
		}
		break;
		
		case CAN1_MS_SPEED_PRESCALE:
		{
			MS_CAN__Run();
		}
		break;
		
		case CAN1_LS_SPEED_PRESCALE:
		{
			LS_CAN__Run();
		}
		break;
		
		default:
		{
		}
		break;
	}
	
}

/*-- EOF --------------------------------------------------------------------*/

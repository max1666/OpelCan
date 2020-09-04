/*-- File description -------------------------------------------------------*/
/**
*   @file:    HS_Can.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "HS_Can.h"
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <string.h>

/*-- Other libraries --------------------------------------------------------*/
#include "defs.h"
#include "debug.h"

/*-- Hardware specific libraries --------------------------------------------*/
#include "CAN1.h"

/*-- Project specific includes ----------------------------------------------*/
#include "SWtimer.h"
#include "RoundBuffer.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint8_t hsCan_IncMsgBuff[(50 * sizeof(CanRxMsg))];
static RoundBuffer_t hsCan_IncMsgBuffer = { hsCan_IncMsgBuff, sizeof(hsCan_IncMsgBuff), 0, 0 };

/*-- Local functions --------------------------------------------------------*/
/*
static void HS_CAN__TransmitMessage(CanTxMsg *message)
{
	CAN1_Transmit(message);
}
*/

/*-- Exported functions -----------------------------------------------------*/
void HS_CAN__ReceiveMessage(CanRxMsg *message)
{
	if(RoundBuffer_GetFree(&hsCan_IncMsgBuffer) >= sizeof(CanRxMsg))
	{
		//Add bytes of struct to round buffer
		RoundBuffer_AddArray(&hsCan_IncMsgBuffer, (uint8_t *)message, sizeof(CanRxMsg));
	}
	else
	{
		Debug_WriteLine("HS-Can: Incoming buffer is FULL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
}

void HS_CAN__Run(void)
{
	uint32_t load = RoundBuffer_GetLoad(&hsCan_IncMsgBuffer);
	
	//Check if we have new messages 
	if(load >= sizeof(CanRxMsg))
	{
		CanRxMsg message;
		
		//Read bytes array to structure
		RoundBuffer_GetArray(&hsCan_IncMsgBuffer, (uint8_t *)&message, sizeof(CanRxMsg));
		
		uint8_t PCI_type = U8_NIBBLE_HIGH(message.Data[0]);
		uint16_t Datalength = 0;
		
		switch(PCI_type)
		{
			//Single frame
			case 0:
			{
				Datalength = U8_NIBBLE_LOW(message.Data[0]);
			}
			break;
			
			//First frame
			case 1:
			{
				Datalength = (U8_NIBBLE_LOW(message.Data[0]) << 8);
				Datalength += message.Data[1];
			}
			break;
			
			//Consecutive frame
			case 2:
			{
				
			}
			break;
			
			//Flow control frame
			case 3:
			{
				//uint8_t framesCount = message.Data[1];
				//uint8_t pause_ms = message.Data[2];
			}
			break;
			
			default:
			{
				
			}
			break;
		}
		
		
		//Debug_WriteLine("  PCI_type: %d", PCI_type);
		//Debug_WriteLine("  Datalength: %d", Datalength);
		
		
		switch(message.StdId)
		{
			default:
			{
				Debug_WriteLine("NMU: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI);
			}
			break;
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

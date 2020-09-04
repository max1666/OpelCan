/*-- File description -------------------------------------------------------*/
/**
*   @file:    MS_Can.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "MS_Can.h"
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
static uint8_t msCan_IncMsgBuff[(50 * sizeof(CanRxMsg))];
static RoundBuffer_t msCan_IncMsgBuffer = { msCan_IncMsgBuff, sizeof(msCan_IncMsgBuff), 0, 0 };

/*-- Local functions --------------------------------------------------------*/
/*
static void MS_CAN__TransmitMessage(CanTxMsg *message)
{
	CAN1_Transmit(message);
}
*/

/*-- Exported functions -----------------------------------------------------*/
void MS_CAN__ReceiveMessage(CanRxMsg *message)
{
	if(RoundBuffer_GetFree(&msCan_IncMsgBuffer) >= sizeof(CanRxMsg))
	{
		//Add bytes of struct to round buffer
		RoundBuffer_AddArray(&msCan_IncMsgBuffer, (uint8_t *)message, sizeof(CanRxMsg));
	}
	else
	{
		Debug_WriteLine("MS-Can: Incoming buffer is FULL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
}

void MS_CAN__Run(void)
{
	uint32_t load = RoundBuffer_GetLoad(&msCan_IncMsgBuffer);
	
	//Check if we have new messages 
	if(load >= sizeof(CanRxMsg))
	{
		CanRxMsg message;
		
		//Read bytes array to structure
		RoundBuffer_GetArray(&msCan_IncMsgBuffer, (uint8_t *)&message, sizeof(CanRxMsg));
		
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
			//Date and Time
			case 0x180:
			{
				uint8_t day = 0;
				uint8_t month = 0;
				uint16_t year = 0;
				
				uint8_t hour = 0;
				uint8_t minute = 0;
				uint8_t seconds = 0;
				
				year = message.Data[2] + 2000;
				month = message.Data[3];
				day = (message.Data[4] >> 3);
				
				hour = (message.Data[4] << 5);
				hour = (hour >> 3);
				hour += (message.Data[5] >> 6);
				
				minute = (message.Data[5] << 2);
				minute = (minute >> 2);
				
				seconds = message.Data[6];
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Date Time: %02d.%02d.%04d %02d:%02d:%02d",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					day, month, year, hour, minute, seconds);
			}
			break;
			
			//Range (Range message displayed on GID)
			case 0x4EE:
			{
				uint32_t range = (message.Data[2] - 40);

				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Range: %d km",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					range);
			}
			break;
			
			//Engine Temperature
			case 0x4EC:
			{
				int32_t temp = (message.Data[2] - 40);

				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Engine Temp: %d C",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					temp);
			}
			break;
			
			//Outdoor Temperature
			case 0x682:
			case 0x683:
			{
				float temp = (message.Data[2] - 80) * 5 / 10.0;
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Outdoor Temp %5.01f C",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					temp);
			}
			break;
			
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

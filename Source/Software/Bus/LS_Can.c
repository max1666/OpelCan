/*-- File description -------------------------------------------------------*/
/**
*   @file:    LS_Can.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "LS_Can.h"
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
static uint8_t lsCan_IncMsgBuff[(50 * sizeof(CanRxMsg))];
static RoundBuffer_t lsCan_IncMsgBuffer = { lsCan_IncMsgBuff, sizeof(lsCan_IncMsgBuff), 0, 0 };

/*-- Local functions --------------------------------------------------------*/
/*
static void LS_CAN__TransmitMessage(CanTxMsg *message)
{
	CAN1_Transmit(message);
}
*/

/*-- Exported functions -----------------------------------------------------*/
void LS_CAN__ReceiveMessage(CanRxMsg *message)
{
	if(RoundBuffer_GetFree(&lsCan_IncMsgBuffer) >= sizeof(CanRxMsg))
	{
		//Add bytes of struct to round buffer
		RoundBuffer_AddArray(&lsCan_IncMsgBuffer, (uint8_t *)message, sizeof(CanRxMsg));
	}
	else
	{
		Debug_WriteLine("LS-Can: Incoming buffer is FULL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
}

void LS_CAN__Run(void)
{
	uint32_t load = RoundBuffer_GetLoad(&lsCan_IncMsgBuffer);
	
	//Check if we have new messages 
	if(load >= sizeof(CanRxMsg))
	{
		CanRxMsg message;
		
		//Read bytes array to structure
		RoundBuffer_GetArray(&lsCan_IncMsgBuffer, (uint8_t *)&message, sizeof(CanRxMsg));
		
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
			//Speed and RPM
			case 0x108:
			{
				uint32_t speed = ((message.Data[4] << 1) & 0xFE) + ((message.Data[5] >> 7) & 0x01 );
				uint16_t rpm = (((message.Data[1] << 8) + message.Data[2] ) >> 2);
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Motion: Speed: %d km/h, RPM: %4d",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					speed, rpm);
			}
			break;
			
			//Pedal status?
			case 0x115:
			{
				uint8_t brakeStatus = BIT_GET(message.Data[0], BIT01); //Bit 1
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Pedals: Brake: %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					brakeStatus);
			}
			break;
			
			//Engine, Coolant, Cruise
			case 0x145:
			{
				int32_t coolantTemp = (message.Data[3] - 40);
				
				uint8_t cruiseStatus = BIT_GET(message.Data[5], BIT01); //Bit 1
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Coolant: %d, Cruise %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					coolantTemp, cruiseStatus);
			}
			break;
			
			//Remote control + Windows ??
			case 0x160:
			{
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Remote control + Windows ?",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI
					);
			}
			break;
			
			//Locks status
			case 0x230:
			{
				uint8_t doors = message.Data[0];  //00-opened 05-closed
				uint8_t bagDoor = message.Data[6];   //00-opened 09-closed

				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Locks status: Doors: %02X, BagDoor: %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					doors, bagDoor);
			}
			break;
			
			//Switches
			case 0x305:
			{
				uint8_t centralLockButton = message.Data[6];  //1-pressed
				
				uint8_t backFogLightsButton = BIT_GET(message.Data[2], BIT04); //Bit 4
				uint8_t frontFogLightsButton = BIT_GET(message.Data[2], BIT05); //Bit 5
				uint8_t switchOnStandingLightsButton = BIT_GET(message.Data[2], BIT06); //Bit 6
				uint8_t switchOnLowBeamLightsButton = BIT_GET(message.Data[2], BIT07); //Bit 7
				
				uint8_t avariikaButton = BIT_GET(message.Data[3], BIT05); //Bit 5

				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Switches Buttons: CentralLock %02X, backFog: %02X, frontFog: %02X, Gabarit: %02X, LowBeam: %02X, Avariika: %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					centralLockButton, backFogLightsButton, frontFogLightsButton, switchOnStandingLightsButton, switchOnLowBeamLightsButton, avariikaButton);
			}
			break;
			
			//Clutch
			case 0x360:
			{
				uint8_t clutchStatus = BIT_GET(message.Data[2], BIT06); //Bit 6
				
				//0 - not pushed
				//1 - pushed
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Clutch status: %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					clutchStatus);
			}
			break;
			
			//Fuel level
			case 0x375:
			{
				uint8_t fuelLevel = message.Data[1];
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Fuel level: %02X",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					fuelLevel);
			}
			break;
			
			//Another Switches ??
			case 0x370:
			{
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Another switches ?",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI
					);
			}
			break;
			
			//Time Date
			case 0x440:
			{
				uint8_t hour = BIT_GET(message.Data[0], (BIT3 | BIT4 | BIT5 | BIT6 | BIT7)) >> 3;
				uint8_t minute = BIT_GET(message.Data[1], (BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7)) >> 2;
				uint8_t seconds = BIT_GET(message.Data[2], (BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7)) >> 2;
				
				uint8_t day = BIT_GET(message.Data[5], (BIT3 | BIT4 | BIT5 | BIT6 | BIT7)) >> 3;
				uint8_t month = BIT_GET(message.Data[6], (BIT4 | BIT5 | BIT6 | BIT7)) >> 4;
				uint16_t year = (BIT_GET(message.Data[7], (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6))) + 2000; //Year limit from 2000 to 2100
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Date Time: %02d.%02d.%04d %02d:%02d:%02d",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					day, month, year, hour, minute, seconds);
			}
			break;
			
			//Outdoor Temperature
			case 0x445:
			{
				float temp = (message.Data[2] / 2.0) - 40;
				
				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Outdoor Temp %5.01f C",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					temp);
			}
			break;
			
			//Voltage
			case 0x500:
			{
				float voltage = message.Data[1] / 8.0;

				Debug_WriteLine("NMF: StdId: %04X, ExtId: %08X, IDE: %02X, RTR: %02X, DLC: %02X, Data: %02X %02X %02X %02X %02X %02X %02X %02X, FMI: %02X.  NMP: Voltage: %5.01f V",
					message.StdId, message.ExtId, message.IDE, message.RTR, message.DLC,
					message.Data[0], message.Data[1], message.Data[2], message.Data[3], message.Data[4], message.Data[5], message.Data[6], message.Data[7],
					message.FMI,
					voltage);
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

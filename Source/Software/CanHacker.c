/*-- File description -------------------------------------------------------*/
/**
*   @file:    CanHacker.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "CanHacker.h"
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <string.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
#include "defs.h"
#include "debug.h"

/*-- Hardware specific libraries --------------------------------------------*/
#include "CAN1.h"
#include "USART3.h"

/*-- Project specific includes ----------------------------------------------*/
#include "RoundBuffer.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
typedef enum
{
	CAH_HACKER__IF_SPEED__10KBPS,
	CAH_HACKER__IF_SPEED__20KBPS,
	CAH_HACKER__IF_SPEED__50KBPS,
	CAH_HACKER__IF_SPEED__100KBPS,
	CAH_HACKER__IF_SPEED__125KBPS,
	CAH_HACKER__IF_SPEED__250KBPS,
	CAH_HACKER__IF_SPEED__500KBPS,
	CAH_HACKER__IF_SPEED__800KBPS,
	CAH_HACKER__IF_SPEED__1000KBPS,
}canHacker_InterfaceSpeed_t;

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint8_t canHacker_IncMsgBuff[(50 * sizeof(CanRxMsg))];
static RoundBuffer_t canHacker_IncMsgBuffer = { canHacker_IncMsgBuff, sizeof(canHacker_IncMsgBuff), 0, 0 };

static uint8_t canHacker_IncUartMsgBuff[256];
static RoundBuffer_t canHacker_IncUartMsgBuffer = { canHacker_IncUartMsgBuff, sizeof(canHacker_IncUartMsgBuff), 0, 0 };

static bool canHacker_CanInterfaceEnabled = false;
static canHacker_InterfaceSpeed_t canHacker_CanInterfaceSpeed = CAH_HACKER__IF_SPEED__500KBPS;
static bool canHacker_CanMessagesTimeStamp = false;

/*-- Local functions --------------------------------------------------------*/

static uint8_t Convert_AsciiHexNum_To_Byte(char symbol)
{
	uint8_t result = 0;
	
	if((symbol >= '0') && (symbol <= '9'))
	{
		result = (uint8_t)symbol - 0x30;
	}
	else if((symbol >= 'A') && (symbol <= 'F'))
	{
		result = symbol - 0x37;
	}
	
	return result;
}

static char Convert__Byte_To_AsciiHexNum(uint8_t halfByte)
{
	char result = 0;
	
	if(halfByte <= 9)
	{
		result = (char)halfByte + 0x30;
	}
	else if((halfByte >= 10) && (halfByte <= 15))
	{
		result = (char)halfByte + 0x37;
	}
	
	return result;
}


static void CanHacker_Can_TransmitMessage(CanTxMsg *message)
{
	CAN1_Transmit(message);
}

static void CanHacker_Uart_ByteArrived(uint8_t byte)
{
	RoundBuffer_AddByte(&canHacker_IncUartMsgBuffer, byte);
}

static void CanHacker_SendResponse(uint8_t *buffer, uint16_t length)
{
	USART3_SendBuf(buffer, length);
}

static void CanHacker_Comand_CanInterfaceEnable(void)
{
	canHacker_CanInterfaceEnabled = true;
}

static void CanHacker_Comand_CanInterfaceDisable(void)
{
	canHacker_CanInterfaceEnabled = false;
}

static void CanHacker_Comand_CanSpeedChange(canHacker_InterfaceSpeed_t newSpeed)
{
	canHacker_CanInterfaceSpeed = newSpeed;
	
	switch(canHacker_CanInterfaceSpeed)
	{
		case CAH_HACKER__IF_SPEED__10KBPS:
		{
			CAN1_ReInit(CAN1_LS_SPEED_PRESCALE, CAN1_LS_TIME_SEG_1, CAN1_LS_TIME_SEG_2);
		}
		break;
		
		case CAH_HACKER__IF_SPEED__100KBPS:
		{
			CAN1_ReInit(CAN1_MS_SPEED_PRESCALE, CAN1_MS_TIME_SEG_1, CAN1_MS_TIME_SEG_2);
		}
		break;
		
		case CAH_HACKER__IF_SPEED__500KBPS:
		{
			CAN1_ReInit(CAN1_HS_SPEED_PRESCALE, CAN1_HS_TIME_SEG_1, CAN1_HS_TIME_SEG_2);
		}
		break;
		
		default:
		{
		}
		break;
	}
}

static void CanHacker_Comand_SetTimeStamp(bool state)
{
	canHacker_CanMessagesTimeStamp = state;
}


static void CanHacker_ParseUartComand(uint8_t *buffer, uint16_t length)
{
	Debug_WriteLine_DumpAscii(buffer, length - 1, "CanHacker:");
	
	switch(buffer[0])
	{
		case 'V':
		{
			const uint8_t response[] = "V0105\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'v':
		{
			const uint8_t response[] = "vSTM32\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'C':
		{
			CanHacker_Comand_CanInterfaceDisable();
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'S':
		{
			if((buffer[1] >= '0') && (buffer[1] <= '8'))
			{
				uint8_t speed = buffer[1] - 0x30;
				
				CanHacker_Comand_CanSpeedChange((canHacker_InterfaceSpeed_t)speed);
			}
			
			const uint8_t response[] = "\r";
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'O':
		{
			CanHacker_Comand_CanInterfaceEnable();
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'Z':
		{
			if(buffer[1] == '1')
			{
				CanHacker_Comand_SetTimeStamp(true);
			}
			else if (buffer[1] == '0')
			{
				CanHacker_Comand_SetTimeStamp(true);
			}
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		//11-Bit CAN Frame
		case 't':
		{
			CanTxMsg message;
			memset(&message, 0x00, sizeof(message));
			
			uint16_t StdId = 0;
			uint8_t dlc = 0;
			uint8_t data[8];
			
			StdId += (Convert_AsciiHexNum_To_Byte(buffer[1]) << 8);
			StdId += (Convert_AsciiHexNum_To_Byte(buffer[2]) << 4);
			StdId += (Convert_AsciiHexNum_To_Byte(buffer[3]) << 0);
			
			dlc += Convert_AsciiHexNum_To_Byte(buffer[4]);
			
			for(uint8_t i = 0, k = 5; i < dlc; i++, k += 2)
			{
				data[i] = 0;
				data[i] += (Convert_AsciiHexNum_To_Byte(buffer[k]) << 4);
				data[i] += (Convert_AsciiHexNum_To_Byte(buffer[k + 1]) << 0);
			}
			
			//Fill structure
			message.StdId = StdId;
			message.IDE = CAN_ID_STD;
			message.RTR = CAN_RTR_DATA;
			message.DLC = dlc;
			memcpy(message.Data, data, dlc);
			
			//Send to CAN bus
			CanHacker_Can_TransmitMessage(&message);
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		//29-Bit CAN Frame
		case 'T':
		{
			CanTxMsg message;
			memset(&message, 0x00, sizeof(message));
			
			uint32_t ExtId = 0;
			uint8_t dlc = 0;
			uint8_t data[8];
			
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[1]) << 28);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[2]) << 24);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[3]) << 20);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[4]) << 16);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[5]) << 12);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[6]) << 8);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[7]) << 4);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[8]) << 0);
			
			dlc += Convert_AsciiHexNum_To_Byte(buffer[9]);
			
			for(uint8_t i = 0, k = 10; i < dlc; i++, k += 2)
			{
				data[i] = 0;
				data[i] += (Convert_AsciiHexNum_To_Byte(buffer[k]) << 4);
				data[i] += (Convert_AsciiHexNum_To_Byte(buffer[k + 1]) << 0);
			}
			
			//Fill structure
			message.StdId = ExtId;
			message.IDE = CAN_ID_EXT;
			message.RTR = CAN_RTR_DATA;
			message.DLC = dlc;
			memcpy(message.Data, data, dlc);
			
			//Send to CAN bus
			CanHacker_Can_TransmitMessage(&message);
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'r':
		{
			CanTxMsg message;
			memset(&message, 0x00, sizeof(message));
			
			uint16_t StdId = 0;
			uint8_t dlc = 0;
			
			StdId += (Convert_AsciiHexNum_To_Byte(buffer[1]) << 8);
			StdId += (Convert_AsciiHexNum_To_Byte(buffer[2]) << 4);
			StdId += (Convert_AsciiHexNum_To_Byte(buffer[3]) << 0);
			
			dlc += Convert_AsciiHexNum_To_Byte(buffer[4]);
			
			//Fill structure
			message.StdId = StdId;
			message.IDE = CAN_ID_STD;
			message.RTR = CAN_RTR_REMOTE;
			message.DLC = dlc;
			
			//Send to CAN bus
			CanHacker_Can_TransmitMessage(&message);
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		case 'R':
		{
			CanTxMsg message;
			memset(&message, 0x00, sizeof(message));
			
			uint32_t ExtId = 0;
			uint8_t dlc = 0;
			
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[1]) << 28);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[2]) << 24);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[3]) << 20);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[4]) << 16);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[5]) << 12);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[6]) << 8);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[7]) << 4);
			ExtId += (Convert_AsciiHexNum_To_Byte(buffer[8]) << 0);
			
			dlc += Convert_AsciiHexNum_To_Byte(buffer[9]);
			
			//Fill structure
			message.ExtId = ExtId;
			message.IDE = CAN_ID_EXT;
			message.RTR = CAN_RTR_REMOTE;
			message.DLC = dlc;
			
			//Send to CAN bus
			CanHacker_Can_TransmitMessage(&message);
			
			const uint8_t response[] = "\r"; 
			CanHacker_SendResponse((uint8_t *)response, sizeof(response));
		}
		break;
		
		default:
		{
			Debug_WriteLine_DumpHex(buffer, length, "CanHacker Unknown:");
		}
	}
}

static void CanHacker_HandleCanMessage(CanRxMsg *message)
{
	static uint8_t canHacker_OutUartMsg[32];
	static uint16_t canHacker_OutUartMsg_Length = 0;
	
	//Clear buffer
	memset(canHacker_OutUartMsg, 0x00, sizeof(canHacker_OutUartMsg));
	canHacker_OutUartMsg_Length = 0;
	
	//Buid
	switch(message->RTR)
	{
		case CAN_RTR_REMOTE:
		{
			switch(message->IDE)
			{
				case CAN_ID_STD:
				{
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = 'r';
					
					//StdId
					//canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGH_U16(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGH_U16(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOW_U16(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOW_U16(message->StdId)));
					
					//DLC
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(message->DLC);
				}
				break;
				
				case CAN_ID_EXT:
				{
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = 'R';
					
					//ExtId
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGHEST_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGHEST_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGH_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGH_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOW_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOW_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOWEST_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOWEST_U32(message->StdId)));
					
					//DLC
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(message->DLC);
				}
				break;
				
				default:
				{
				}
				break;
			}
		}
		break;
		
		case CAN_RTR_DATA:
		{
			switch(message->IDE)
			{
				case CAN_ID_STD:
				{
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = 't';
					
					//StdId
					//canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGH_U16(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGH_U16(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOW_U16(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOW_U16(message->StdId)));
					
					//DLC
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(message->DLC);
					
					//Data
					for(uint8_t i = 0; i < message->DLC; i++)
					{
						canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(message->Data[i]));
						canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(message->Data[i]));
					}
				}
				break;
				
				case CAN_ID_EXT:
				{
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = 'T';
					
					//ExtId
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGHEST_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGHEST_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGH_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGH_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOW_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOW_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOWEST_U32(message->StdId)));
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOWEST_U32(message->StdId)));
					
					//DLC
					canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(message->DLC);
					
					//Data
					for(uint8_t i = 0; i < message->DLC; i++)
					{
						canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(message->Data[i]));
						canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(message->Data[i]));
					}
				}
				break;
				
				default:
				{
				}
				break;
			}
		}
		break;
		
		default:
		{
		}
		break;
	}
	
	//Timestamp if needed
	if(canHacker_CanMessagesTimeStamp)
	{
		uint16_t time_ms = 0;
		uint32_t ticks = Debug_GetTick(); //ms counter
		
		uint32_t cnt = ticks / 60000;
		
		
		time_ms = ticks - (cnt * 60000);
		
		
		//TimeStamp
		canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(HIGH_U16(time_ms)));
		canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(HIGH_U16(time_ms)));
		canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_HIGH(LOW_U16(time_ms)));
		canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = Convert__Byte_To_AsciiHexNum(U8_NIBBLE_LOW(LOW_U16(time_ms)));
	}
	
	canHacker_OutUartMsg[canHacker_OutUartMsg_Length++] = '\r';
	
	//Send to UART
	CanHacker_SendResponse(canHacker_OutUartMsg, canHacker_OutUartMsg_Length);
}

/*-- Exported functions -----------------------------------------------------*/
void CanHacker_Init(void)
{
	USART3_Init(CanHacker_Uart_ByteArrived, NULL);
}

void CanHacker_ReceiveCanMessage(CanRxMsg *message)
{
	if(RoundBuffer_GetFree(&canHacker_IncMsgBuffer) >= sizeof(CanRxMsg))
	{
		//Add bytes of struct to round buffer
		RoundBuffer_AddArray(&canHacker_IncMsgBuffer, (uint8_t *)message, sizeof(CanRxMsg));
	}
	else
	{
		Debug_WriteLine("CanHacker: Incoming buffer is FULL !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
	}
}


void CanHacker_Run(void)
{
	static uint8_t canHacker_IncUartMsg[32];
	static uint16_t canHacker_IncUartMsg_Length = 0;
	
	uint32_t loadUart = RoundBuffer_GetLoad(&canHacker_IncUartMsgBuffer);
	
	//Check if we have new messages 
	if(loadUart > 0)
	{
		uint8_t byte = RoundBuffer_GetByte(&canHacker_IncUartMsgBuffer);
		canHacker_IncUartMsg[canHacker_IncUartMsg_Length++] = byte;
		
		if(byte == '\r')
		{
			//Parse
			CanHacker_ParseUartComand(canHacker_IncUartMsg, canHacker_IncUartMsg_Length);
			
			//Clear buffer
			memset(canHacker_IncUartMsg, 0x00, sizeof(canHacker_IncUartMsg));
			canHacker_IncUartMsg_Length = 0;
		}
	}
	
	//-------------------------------------------------------------------------
	
	uint32_t loadCan = RoundBuffer_GetLoad(&canHacker_IncMsgBuffer);
	
	//Check if we have new messages 
	if(loadCan >= sizeof(CanRxMsg))
	{
		CanRxMsg message;
		
		//Read bytes array to structure
		RoundBuffer_GetArray(&canHacker_IncMsgBuffer, (uint8_t *)&message, sizeof(CanRxMsg));
		
		//Handle
		if(canHacker_CanInterfaceEnabled)
		{
			CanHacker_HandleCanMessage(&message);
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

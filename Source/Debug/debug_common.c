/*-- File description -------------------------------------------------------*/
/**
*   @file:    debug_common.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "debug_common.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>
/*-- Other libraries --------------------------------------------------------*/
#include "common.h"
#include "debug.h"
#include "RoundBuffer.h"
#include "SWtimer.h"
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define DBG_RX_BUF_SIZE         256

/*-- Local variables --------------------------------------------------------*/
static uint8_t _uartRxBuff[DBG_RX_BUF_SIZE] = { 0 };
static RoundBuffer_t DBG_RxBuffer = {_uartRxBuff, DBG_RX_BUF_SIZE, 0, 0};

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void GetInfo(void)
{
	Debug_WriteLine("GetInfo()");
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void SWtimer_ParseDebug(SWtimerParam_t params)
{
	uint8_t buff[2] = { 0, 0 };

	uint8_t RxBuffer[DBG_RX_BUF_SIZE];
	uint32_t RxLength = 0;

	while(RoundBuffer_GetLoad(&DBG_RxBuffer) > 0)
	{
		uint8_t byte = RoundBuffer_GetByte(&DBG_RxBuffer);

		buff[0] = buff[1];
		buff[1] = byte;

		RxBuffer[RxLength++] = byte;

		if((buff[0] == '\r') && (buff[1] == '\n'))
		{
			break;
		}
	}


	if(RxLength > 2)
	{
		if((RxBuffer[RxLength - 2] == '\r') && (RxBuffer[RxLength - 1] == '\n'))
		{
			char *head = NULL;
			uint32_t msgLength = RxLength - 2;
			RxBuffer[RxLength - 1] = '\0';
			RxBuffer[RxLength - 2] = '\0';
			RxLength = 0;


			head = RIL_FindString((char *)RxBuffer, msgLength, "GetInfo");
			if(head)
			{
				GetInfo();
				return;
			}

			head = RIL_FindString((char *)RxBuffer, msgLength, "PrintCritical");
			if(head)
			{
				Debug_CriticalPrint();
				return;
			}

			Debug_WriteLine("Unhandled Message of %d bytes: '%s'", msgLength, RxBuffer);
		}
	}
}

/*-- Exported functions -----------------------------------------------------*/
/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_ByteArrived(uint8_t byte)
{
	static uint8_t buff[2] = {0, 0};

	RoundBuffer_AddByte(&DBG_RxBuffer, byte);

	buff[0] = buff[1];
	buff[1] = byte;

	if((buff[0] == '\r') && (buff[1] == '\n'))
	{
		SWtimer_RunTimerCallback(SWTT_One_low, 0, 5, SWtimer_ParseDebug, NULL);
	}
}

/*-- EOF --------------------------------------------------------------------*/

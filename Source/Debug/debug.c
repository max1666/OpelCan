/*-- File description -------------------------------------------------------*/
/**
*   @file:    debug.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "debug.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>
#include <stdbool.h>

/*-- Other libraries --------------------------------------------------------*/
#include "printf_user.h"

/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
#include "RoundBuffer.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
#define DEBUG_MESSAGE_BUFFER_SIZE               256
#define DEBUG_CRITICAL_BUFFER_SIZE              1024

/*-- Local variables --------------------------------------------------------*/
static uint32_t debugMsCounter = 0;
static bool criticalInfoSaveFlag = false;
static uint8_t _criticalBuff[DEBUG_CRITICAL_BUFFER_SIZE] = { 0 };
static RoundBuffer_t DBG_CriticalBuffer = {_criticalBuff, DEBUG_CRITICAL_BUFFER_SIZE, 0, 0};

static DebugWriteFunction writeFunc = NULL;

/*-- Local function prototypes ----------------------------------------------*/
void Debug_Init(DebugWriteFunction func);
void Debug_Tick(uint32_t time_ms);
void Debug_PrintByte(uint8_t byte);
void Debug_PrintBuffer(uint8_t *buffer, uint16_t length);
void Debug_PrintTime(void);

void Debug_Write(const char *format, ...);
void Debug_Write_Dump(uint8_t *array, uint16_t length, const char *format, ...);
void Debug_WriteLine(const char *format, ...);
void Debug_WriteLine_DumpHex(uint8_t *array, uint16_t length, const char *format, ...);
void Debug_WriteLine_DumpAscii(uint8_t *array, uint16_t length, const char *format, ...);

/*-- Local functions --------------------------------------------------------*/
/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_Write_va(const char *format, va_list ap)
{
    uint8_t buffer[DEBUG_MESSAGE_BUFFER_SIZE];
    uint16_t length = 0;
    memset(buffer, 0x00, DEBUG_MESSAGE_BUFFER_SIZE);

    length = vsnprintf((char *)buffer, DEBUG_MESSAGE_BUFFER_SIZE, format, ap);

    Debug_PrintBuffer(buffer, length);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_WriteToBuffer_va(uint8_t *buffer, uint16_t *length, const char *format, va_list ap)
{
	if((buffer) && (length))
	{
		*length += vsnprintf((char *)buffer, DEBUG_MESSAGE_BUFFER_SIZE, format, ap);
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
void Debug_Init(DebugWriteFunction func)
{
	if(func)
	{
		writeFunc = func;
		Debug_WriteLine("Debug UART Initialized.");
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
inline void Debug_Tick(uint32_t time_ms)
{
	debugMsCounter += time_ms;
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
inline uint32_t Debug_GetTick(void)
{
	return debugMsCounter;
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_PrintByte(uint8_t byte)
{
	if(criticalInfoSaveFlag)
	{
		RoundBuffer_AddByte(&DBG_CriticalBuffer, byte);
	}

	if(writeFunc)
	{
		writeFunc(&byte, 1);
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_PrintBuffer(uint8_t *buffer, uint16_t length)
{
	if(criticalInfoSaveFlag)
	{
		RoundBuffer_AddArray(&DBG_CriticalBuffer, buffer, length);
	}

	if(writeFunc)
	{
		writeFunc(buffer, length);
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_PrintTime(void)
{
	uint8_t buffer[16];
	uint16_t length = 0;
	memset(buffer, 0x00, 16);

	length = sprintf((char *)buffer, "%07d.%03d: ", (int)(debugMsCounter / 1000), (int)(debugMsCounter % 1000));

	Debug_PrintBuffer(buffer, length);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_PrintTime_ToBuffer(uint8_t *buffer, uint16_t *length)
{
	if((buffer) && (length))
	{
		*length += sprintf((char *)buffer, "%07d.%03d: ", (int)(debugMsCounter / 1000), (int)(debugMsCounter % 1000));
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_CriticalEnable(void)
{
	Debug_WriteLine("----------Debug_CriticalEnable()----------");
	criticalInfoSaveFlag = true;
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_CriticalDisable(void)
{
	criticalInfoSaveFlag = false;
	Debug_WriteLine("----------Debug_CriticalDisable()----------");
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_CriticalPrint(void)
{
	if(writeFunc)
	{
		while (RoundBuffer_GetLoad(&DBG_CriticalBuffer) > 0)
		{
			uint8_t byte = RoundBuffer_GetByte(&DBG_CriticalBuffer);
			
			writeFunc(&byte, 1);
		}
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_Write(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    Debug_Write_va(format, ap);

    va_end(ap);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_Write_ToBuffer(uint8_t *buffer, uint16_t *length, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

    Debug_WriteToBuffer_va(buffer, length, format, ap);

    va_end(ap);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_Write_Dump(uint8_t *array, uint16_t length, const char *format, ...)
{
	va_list ap;
	va_start(ap, format);

	Debug_Write_va(format, ap);

	uint16_t i = 0;
	for(i = 0; i < length; i++)
	{
		Debug_Write(" %02X", array[i]);
	}

	va_end(ap);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_WriteLine(const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

	Debug_PrintTime();
	Debug_Write_va(format, ap);
    Debug_Write("\r\n");

    va_end(ap);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_WriteLine_ToBuffer(uint8_t *buffer, uint16_t *length, const char *format, ...)
{
    va_list ap;
    va_start(ap, format);

	if((buffer) && (length))
	{
		*length = 0;
		
		Debug_Write_ToBuffer(buffer, length, format, ap);
		Debug_Write_ToBuffer(buffer + *length, length, "\r\n");
	}
	
    va_end(ap);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_WriteLine_DumpHex(uint8_t *array, uint16_t length, const char *format, ...)
{
	if(array)
	{
		va_list ap;
		va_start(ap, format);

		Debug_PrintTime();
		Debug_Write_va(format, ap);

		uint16_t i = 0;
		for(i = 0; i < length; i++)
		{
			Debug_Write(" %02X", array[i]);
		}

		Debug_Write("\r\n");

		va_end(ap);
	}
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
void Debug_WriteLine_DumpAscii(uint8_t *array, uint16_t length, const char *format, ...)
{
	if(array)
	{
		va_list ap;
		va_start(ap, format);

		Debug_PrintTime();
		Debug_Write_va(format, ap);

		Debug_Write("\"");

		uint16_t i = 0;
		for(i = 0; i < length; i++)
		{
			Debug_Write("%c", array[i]);
		}

		Debug_Write("\"");

		Debug_Write("\r\n");

		va_end(ap);
	}
}

/*-- EOF --------------------------------------------------------------------*/

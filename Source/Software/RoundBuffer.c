/*-- File description -------------------------------------------------------*/
/**
*   @file:    RoundBuffer.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "RoundBuffer.h"
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <string.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
/******************************************************************************
 *  @brief  Add byte to round buffer.
 *
 *  @param  buffer - pointer to round buffer.
 *  @param  byte - byte to add.
 *
 *  @retval None.
 *****************************************************************************/
void RoundBuffer_AddByte(RoundBuffer_t *buffer, uint8_t byte)
{
	if(buffer)
	{
		buffer->Buff[buffer->Head++] = byte;
		
		if (buffer->Head >= buffer->Size)
		{
			buffer->Head = 0;
		}
	}
}

/******************************************************************************
 *  @brief  Add byte array to round buffer.
 *
 *  @param  buffer - pointer to round buffer.
 *  @param  inArray - pointer to byte array.
 *  @param  length - count of bytes to add.
 *
 *  @retval None.
 *****************************************************************************/
void RoundBuffer_AddArray(RoundBuffer_t *buffer, uint8_t *inArray, uint32_t length)
{
	if((buffer) && (inArray) && (length > 0))
	{
		uint32_t i = 0;
		for(i = length; i > 0; i--)
		{
			buffer->Buff[buffer->Head++] = *inArray++;
			if(buffer->Head >= buffer->Size)
			{
				buffer->Head = 0;
			}
		}
	}
}

/******************************************************************************
 *  @brief  Clear the round buffer.
 *
 *  @param  buffer - pointer to round buffer.
 *
 *  @retval None.
 *****************************************************************************/
void RoundBuffer_Clear(RoundBuffer_t *buffer)
{
	if(buffer)
	{
		buffer->Tail = 0;
		buffer->Head = 0;
		memset(buffer->Buff, 0, buffer->Size);
	}
}

/******************************************************************************
 *  @brief  Get one byte from round buffer.
 *
 *  @param  buffer - pointer to round buffer.
 *
 *  @retval byte from round buffer.
 *****************************************************************************/
uint8_t RoundBuffer_GetByte(RoundBuffer_t *buffer)
{
	uint8_t data = 0x00;
	
	if (buffer)
	{
		if(RoundBuffer_GetLoad(buffer) > 0)
		{
			uint32_t tail = buffer->Tail;
			
			data = buffer->Buff[tail++];

			if(tail >= buffer->Size)
			{
				tail = 0;
			}

			buffer->Tail = tail;
		}
	}
	
	return data;
}

/******************************************************************************
 *  @brief  Get byte array from round buffer.
 *
 *  @param  buffer - pointer to round buffer.
 *  @param  outArray - pointer to byte array read to.
 *  @param  length - count of bytes to read.
 *
 *  @retval bytes count readed from buffer.
 *****************************************************************************/
uint32_t RoundBuffer_GetArray(RoundBuffer_t *buffer, uint8_t *outArray, uint32_t length)
{
	uint32_t getBytes = 0;

	if((buffer) && (outArray) && (length > 0))
	{
		uint32_t buffLoad = RoundBuffer_GetLoad(buffer);

		if(buffLoad > 0)
		{
			uint32_t i = 0;

			//Calculating
			if(buffLoad > length)
			{
				getBytes = length;
			}
			else
			{
				getBytes = buffLoad;
			}

			//Reading
			for(i = 0; i < getBytes; i++)
			{
				outArray[i] = RoundBuffer_GetByte(buffer);
			}
		}
	}

	return getBytes;
}

uint32_t RoundBuffer_GetSize(RoundBuffer_t *buffer)
{
	uint32_t result = 0;
	
	if(buffer)
	{
		result = buffer->Size;
	}
	
	return result;
}

/******************************************************************************
 *  @brief  Get count of bytes in round buffer.
 *
 *  @param  buffer - pointer to round buffer.
 *
 *  @retval bytes count in buffer.
 *****************************************************************************/
uint32_t RoundBuffer_GetLoad(RoundBuffer_t *buffer)
{
	uint32_t result = 0;
	
	if(buffer)
	{
		int32_t bytes_num = buffer->Head - buffer->Tail;

		if(bytes_num < 0)
		{
			bytes_num += buffer->Size;
		}
		
		result = (uint32_t)bytes_num;
	}
	
	return result;
}

uint32_t RoundBuffer_GetFree(RoundBuffer_t *buffer)
{
	uint32_t result = 0;
	
	if(buffer)
	{		
		result = (uint32_t)(buffer->Size - RoundBuffer_GetLoad(buffer));
	}
	
	return result;
}

/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
*   @file:    RoundBuffer.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef ROUND_BUFFER_H
#define ROUND_BUFFER_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef struct
{
	uint8_t *Buff;
	uint32_t Size;
    uint32_t Tail;
    uint32_t Head;
}RoundBuffer_t;

/*-- Exported functions -----------------------------------------------------*/
void RoundBuffer_AddByte(RoundBuffer_t *buffer, uint8_t byte);
void RoundBuffer_AddArray(RoundBuffer_t *buffer, uint8_t *inArray, uint32_t length);
void RoundBuffer_Clear(RoundBuffer_t *buffer);
uint32_t RoundBuffer_GetSize(RoundBuffer_t *buffer);
uint32_t RoundBuffer_GetLoad(RoundBuffer_t *buffer);
uint32_t RoundBuffer_GetFree(RoundBuffer_t *buffer);
uint32_t RoundBuffer_GetArray(RoundBuffer_t *buffer, uint8_t *outArray, uint32_t length);
uint8_t RoundBuffer_GetByte(RoundBuffer_t *buffer);

#endif // ROUND_BUFFER_H
/*-- EOF --------------------------------------------------------------------*/

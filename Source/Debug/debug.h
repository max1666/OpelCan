/*-- File description -------------------------------------------------------*/
/**
*   @file:    debug.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef _DEBUG_H_
#define _DEBUG_H_
/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef void (*DebugWriteFunction)(uint8_t *buffer, uint16_t size);

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void Debug_Init(DebugWriteFunction func);
void Debug_Tick(uint32_t time_ms);
uint32_t Debug_GetTick(void);
void Debug_PrintByte(uint8_t byte);
void Debug_PrintBuffer(uint8_t *buffer, uint16_t length);
void Debug_PrintTime(void);

void Debug_CriticalEnable(void);
void Debug_CriticalDisable(void);
void Debug_CriticalPrint(void);

void Debug_Write(const char *format, ...);
void Debug_Write_Dump(uint8_t *array, uint16_t length, const char *format, ...);
void Debug_WriteLine(const char *format, ...);
void Debug_WriteLine_DumpHex(uint8_t *array, uint16_t length, const char *format, ...);
void Debug_WriteLine_DumpAscii(uint8_t *array, uint16_t length, const char *format, ...);


void Debug_PrintTime_ToBuffer(uint8_t *buffer, uint16_t *length);
void Debug_Write_ToBuffer(uint8_t *buffer, uint16_t *length, const char *format, ...);
void Debug_WriteLine_ToBuffer(uint8_t *buffer, uint16_t *length, const char *format, ...);

#endif // _DEBUG_H_
/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
*   @file:    common.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef _COMMON_H
#define _COMMON_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void System_Reset(void);

char *RIL_FindString(char *line, uint32_t len, char *str);

uint32_t HexCharStrToInt(char *hex);
int32_t DecCharStrToInt(char *dec);
uint32_t DecCharStrToUint(char *dec);

int32_t PowFast(int32_t num, int32_t deg);
#endif // _COMMON_H
/*-- EOF --------------------------------------------------------------------*/

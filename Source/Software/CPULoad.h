/*-- File description -------------------------------------------------------*/
/**
*   @file:    CPULoad.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef CPU_LOAD_H
#define CPU_LOAD_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#define CPU_LOAD_0_TICKS                83500

/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
static void CPULoad_Calculate(void);
void CPULoad_Init(void);
float CPULoad_Read(void);

void CPULoad_Tick_ms(uint32_t ms);
void CPULoad_Tick_Main(void);

#endif // CPU_LOAD_H
/*-- EOF --------------------------------------------------------------------*/

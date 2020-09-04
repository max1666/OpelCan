/*-- File description -------------------------------------------------------*/
/**
*   @file:    CanHacker.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef CAN_HACKER_H
#define CAN_HACKER_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_can.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void CanHacker_Init(void);

void CanHacker_ReceiveCanMessage(CanRxMsg *message);


void CanHacker_Run(void);

#endif // CAN_HACKER_H
/*-- EOF --------------------------------------------------------------------*/

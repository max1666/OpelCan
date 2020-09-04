/*-- File description -------------------------------------------------------*/
/**
*   @file:    LS_Can.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef LS_CAN_H
#define LS_CAN_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_can.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void LS_CAN__ReceiveMessage(CanRxMsg *message);
void LS_CAN__Run(void);

#endif // LS_CAN_H
/*-- EOF --------------------------------------------------------------------*/

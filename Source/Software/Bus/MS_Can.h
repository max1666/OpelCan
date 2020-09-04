/*-- File description -------------------------------------------------------*/
/**
*   @file:    MS_Can.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef MS_CAN_H
#define MS_CAN_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_can.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void MS_CAN__ReceiveMessage(CanRxMsg *message);
void MS_CAN__Run(void);

#endif // MS_CAN_H
/*-- EOF --------------------------------------------------------------------*/

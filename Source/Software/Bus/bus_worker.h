/*-- File description -------------------------------------------------------*/
/**
*   @file:    bus_worker.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef BUS_WORKER_H
#define BUS_WORKER_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_can.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void CanBus_ReceiveMessage(CanRxMsg *message);

void CanBus_Run(void);

#endif // BUS_WORKER_H
/*-- EOF --------------------------------------------------------------------*/

/*-- File description -------------------------------------------------------*/
/**
*   @file:    USB0.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef _USB_0_H
#define _USB_0_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void USB0_Init(void);
void USB0_DeInit(void);

void USB0_InterruptHandler(void);

#endif // _USB_0_H
/*-- EOF --------------------------------------------------------------------*/

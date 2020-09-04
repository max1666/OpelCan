/*-- File description -------------------------------------------------------*/
/**
*   @file:    usb_vcp.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef _USB_VCP_H
#define _USB_VCP_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void VCP_Send(uint8_t *buffer, uint32_t length);

void VCP_DataSendedCallback(void);
void VCP_DataReceivedCallback(uint8_t *buffer, uint32_t length);

#endif // _USB_VCP_H
/*-- EOF --------------------------------------------------------------------*/

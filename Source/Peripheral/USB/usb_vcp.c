/*-- File description -------------------------------------------------------*/
/**
*   @file:    usb_vcp.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "usb_vcp.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "usb_desc.h"

#include "usb_lib.h"
#include "usb_istr.h"
#include "usb_pwr.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
bool sending_in_process = FALSE;

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void VCP_Send(uint8_t *buffer, uint32_t length)
{
	// if max buffer is Not reached
	if(length < VIRTUAL_COM_PORT_DATA_SIZE)
	{
		if(sending_in_process == FALSE)
		{
			// Sent flag
			sending_in_process = TRUE;
			
			// Dsend  packet to PMA
			UserToPMABufferCopy(buffer, ENDP1_TXADDR, (uint16_t)length);
			SetEPTxCount(ENDP1, (uint16_t)length);
			SetEPTxValid(ENDP1);
		}
	}
}

void VCP_DataSendedCallback(void)
{
	sending_in_process = FALSE;
}

void VCP_DataReceivedCallback(uint8_t *buffer, uint32_t length)
{
	VCP_Send(buffer, length);
}

/*-- EOF --------------------------------------------------------------------*/

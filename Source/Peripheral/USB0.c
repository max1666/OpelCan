/*-- File description -------------------------------------------------------*/
/**
*   @file:    USB0.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "USB0.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "misc.h"

#include "usb_lib.h"


#include "usb_istr.h"
#include "usb_pwr.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
static void RCC_Periph_Init(void)
{
	//Enable USB Port
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	//Enable USB disconnect GPIO clock
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
	
	// Select USBCLK source
	RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);
	
	// Enable the USB clock
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

static void RCC_Periph_DeInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE);
}

static void GPIO_Periph_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// USB DP and DM pins
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 | GPIO_Pin_12;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	// USB_DISCONNECT used as USB pull-up
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
}

static void GPIO_Periph_DeInit(void)
{

}

static void NVIC_Periph_Init(void)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// 2 bit for pre-emption priority, 2 bits for subpriority
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); 
	
	NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	
	// Enable the USB Wake-up interrupt
	NVIC_InitStructure.NVIC_IRQChannel = USBWakeUp_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_Init(&NVIC_InitStructure);
}

static void NVIC_Periph_DeInit(void)
{
	NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
}

static void USB_Periph_Init(void)
{
	USB_Init();
}

static void USB_Periph_DeInit(void)
{

}

/*-- Exported functions -----------------------------------------------------*/
void USB0_Init(void)
{
    RCC_Periph_Init();
	GPIO_Periph_Init();
	NVIC_Periph_Init();
	USB_Periph_Init();
}

void USB0_DeInit(void)
{
	USB_Periph_DeInit();
	NVIC_Periph_DeInit();
	GPIO_Periph_DeInit();
	RCC_Periph_DeInit();
}

void USB0_InterruptHandler(void)
{
	USB_Istr();
}

/*-- EOF --------------------------------------------------------------------*/

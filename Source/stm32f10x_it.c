/*-- File description -------------------------------------------------------*/
/**
*   @file:    stm32f10x_it.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "stm32f10x_it.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_can.h"
#include "stm32f10x_exti.h"
#include "stm32f10x_usart.h"

/*-- Project specific includes ----------------------------------------------*/
#include "TIM4.h"
#include "CAN1.h"
#include "USB0.h"
#include "USART1.h"
#include "USART3.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void NMI_Handler(void)
{
	
}

void HardFault_Handler(void)
{
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	while (1)
	{
	}
}

void SVC_Handler(void)
{
	
}

void DebugMon_Handler(void)
{
	
}

void PendSV_Handler(void)
{
	
}

void SysTick_Handler(void)
{
	
}

void TIM4_IRQHandler(void)
{
	TIM4_InterruptHandler();
}

//USB Device Low Priority or CAN1 RX0 Interrupts
void USB_LP_CAN1_RX0_IRQHandler(void)
{
	if((CAN_GetITStatus(CAN1, CAN_IT_TME) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_FF0) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_FOV0) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_FMP1) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_FF1) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_FOV1) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_WKU) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_SLK) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_EWG) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_EPV) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_BOF) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_LEC) == SET) ||
		(CAN_GetITStatus(CAN1, CAN_IT_ERR) == SET))
	{
		CAN1_InterruptHandler();
	}
	else
	{
		USB0_InterruptHandler();
	}
}

//CAN1 RX1 Interrupt
void CAN1_RX1_IRQHandler(void)
{
	
}

//USB Device High Priority or CAN1 TX Interrupts
void USB_HP_CAN1_TX_IRQHandler(void)
{
	
}

void USBWakeUp_IRQHandler(void)
{
	EXTI_ClearITPendingBit(EXTI_Line18);
}

void USART1_IRQHandler(void)
{
	//?????????? ?? ?????????? ?????? ?????? ??????
    if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)
    {
		USART1_InterruptHandler();
	}
	//?????????? ?? ?????? ???????? ??????
    if(USART_GetITStatus(USART1, USART_IT_TXE) != RESET)
    {   
		USART1_InterruptHandler();
	}
	//?????????? ?? ?????????? ????????
    if(USART_GetITStatus(USART1, USART_IT_TC) != RESET)
    { 
		USART1_InterruptHandler();
	}
	//?????????? ?? ??????? ?????? ? ???????? ??????
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{ 
		USART1_InterruptHandler();
	}
	//?????????? ?? ???????????? ??????? ??????
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{ 
		USART1_InterruptHandler();
	}
}

void USART3_IRQHandler(void)
{
	USART3_InterruptHandler();
}

/*-- EOF --------------------------------------------------------------------*/

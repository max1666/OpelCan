/*-- File description -------------------------------------------------------*/
/**
*   @file:    stm32f10x_it.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef __STM32F10x_IT_H
#define __STM32F10x_IT_H

#ifdef __cplusplus
 extern "C" {
#endif 

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void NMI_Handler(void);
void HardFault_Handler(void);
void MemManage_Handler(void);
void BusFault_Handler(void);
void UsageFault_Handler(void);
void SVC_Handler(void);
void DebugMon_Handler(void);
void PendSV_Handler(void);
void SysTick_Handler(void);

#ifdef __cplusplus
}
#endif

#endif // __STM32F10x_IT_H
/*-- EOF --------------------------------------------------------------------*/

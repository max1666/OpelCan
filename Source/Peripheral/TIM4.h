/*-- File description -------------------------------------------------------*/
/**
*   @file:    TIM4.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef _TIM_4_INTERNAL_H
#define _TIM_4_INTERNAL_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef void (*HwTimer_4_Callback)(void);

/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void TIM4_Init(void);
void TIM4_DeInit(void);
void TIM4_Start(void);
void TIM4_Stop(void);
void TIM4_RegisterInterruptCallback(HwTimer_4_Callback callback);
void TIM4_UnRegisterInterruptCallback(void);

void TIM4_InterruptHandler(void);

#endif // _TIM_4_INTERNAL_H
/*-- EOF --------------------------------------------------------------------*/

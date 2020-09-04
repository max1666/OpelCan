/*-- File description -------------------------------------------------------*/
/**
*   @file:    TIM4.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "TIM4.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static HwTimer_4_Callback interruptCallback = NULL;

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
static void TIMER_RCC_Init(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
}

static void TIMER_RCC_DeInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, DISABLE);
}

static void TIMER_NVIC_Init(void)
{
	NVIC_SetPriority(TIM4_IRQn, 15);
    NVIC_EnableIRQ(TIM4_IRQn);
}

static void TIMER_NVIC_DeInit(void)
{
	NVIC_DisableIRQ(TIM4_IRQn);
}

static void TIMER_Init(void)
{
	TIM_TimeBaseInitTypeDef TIMER_InitStructure;
    
    TIM_TimeBaseStructInit(&TIMER_InitStructure);
    TIMER_InitStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIMER_InitStructure.TIM_Prescaler = 720 -1;
    TIMER_InitStructure.TIM_Period = 100 - 1;
    TIM_TimeBaseInit(TIM4, &TIMER_InitStructure);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
}

static void TIMER_DeInit(void)
{
	TIM_ITConfig(TIM4, TIM_IT_Update, DISABLE);
	TIM_DeInit(TIM4);
}

static void TIMER_Start(void)
{
	TIM_Cmd(TIM4, ENABLE);
}

static void TIMER_Stop(void)
{
	TIM_Cmd(TIM4, DISABLE);
}

/*-- Exported functions -----------------------------------------------------*/
void TIM4_Init(void)
{
    TIMER_RCC_Init();
	TIMER_NVIC_Init();
	TIMER_Init();
}

void TIM4_DeInit(void)
{
	TIMER_Stop();
	TIMER_DeInit();
	TIMER_NVIC_DeInit();
	TIMER_RCC_DeInit();
}

void TIM4_Start(void)
{
	TIMER_Start();
}
	
void TIM4_Stop(void)
{
	TIMER_Stop();
}

void TIM4_RegisterInterruptCallback(HwTimer_4_Callback callback)
{
	interruptCallback = callback;
}

void TIM4_UnRegisterInterruptCallback(void)
{
	interruptCallback = NULL;
}

void TIM4_InterruptHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
		
		if(interruptCallback)
		{
			interruptCallback();
		}
	}
}

/*-- EOF --------------------------------------------------------------------*/

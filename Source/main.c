/*-- File description -------------------------------------------------------*/
/**
*   @file:    main.c
*
*   @author:  valeriy.williams
*   @company: Lab.
*/

#include "main.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
int main(void)
{
	// GPIOC Periph clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Configure PC13 in output pushpull mode
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	// Infinite loop
	while (1)
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		
		for(uint32_t i = 0; i < 500000; i++);
		
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		
		for(uint32_t i = 0; i < 500000; i++);
	}
}

/*-- EOF --------------------------------------------------------------------*/

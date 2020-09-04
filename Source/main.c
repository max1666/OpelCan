/*-- File description -------------------------------------------------------*/
/**
*   @file:    main.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "main.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <stdbool.h>
#include <string.h>

/*-- Other libraries --------------------------------------------------------*/
#include "TIM4.h"
#include "CAN1.h"
#include "USB0.h"
#include "USART1.h"

/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/*-- Project specific includes ----------------------------------------------*/
#include "SWtimer.h"
#include "debug.h"
#include "debug_common.h"
#include "bus_worker.h"
#include "CanHacker.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
void timer4_Tick(void)
{
	//CPULoad_Tick_ms(1);
	Debug_Tick(1);
	SWtimer_TimerTick_HighPriority(1);
}

void pinToggleTimerCb(SWtimerParam_t params)
{
	static bool state = false;

	if(state)
	{
		GPIO_ResetBits(GPIOC, GPIO_Pin_13);
		state = false;
	}
	else
	{
		GPIO_SetBits(GPIOC, GPIO_Pin_13);
		state = true;
	}
}

void RxDebugCmdTimerCb(SWtimerParam_t params)
{
	/*
	CanRxMsg RxMessage;
	
	RxMessage.StdId = 0x0180;
	RxMessage.ExtId = 0x00000000;
	RxMessage.IDE = 0x00;
	RxMessage.RTR = 0x00;
	RxMessage.DLC = 0x00;
	RxMessage.Data [0] = 0x46;
	RxMessage.Data [1] = 0x03;
	RxMessage.Data [2] = 0x08;
	RxMessage.Data [3] = 0x07;
	RxMessage.Data [4] = 0xd0;
	RxMessage.Data [5] = 0x6e;
	RxMessage.Data [6] = 0x24;
	RxMessage.Data [7] = 0x00;
	RxMessage.FMI = 0x00;
	
	CanBus_ReceiveMessage(&RxMessage);
	*/
	
	CanRxMsg RxMessage;
	
	RxMessage.StdId = 0x0440;
	//RxMessage.StdId = 0x120B;
	RxMessage.ExtId = 0x00000000;
	RxMessage.IDE = CAN_ID_STD;
	RxMessage.RTR = CAN_RTR_DATA;
	//RxMessage.RTR = CAN_RTR_REMOTE;
	RxMessage.DLC = 0x08;
	RxMessage.Data [0] = 0x88;
	RxMessage.Data [1] = 0x50;
	RxMessage.Data [2] = 0x30;
	RxMessage.Data [3] = 0x33;
	RxMessage.Data [4] = 0x00;
	RxMessage.Data [5] = 0x20;
	RxMessage.Data [6] = 0x90;
	RxMessage.Data [7] = 0x14;
	RxMessage.FMI = 0x00;
	
	CanBus_ReceiveMessage(&RxMessage);
}


void DebugSendFunc(uint8_t *buffer, uint16_t length)
{
	static bool overload = false;
	static bool overloadSended = false;
	
	//uint32_t size = USART1_TxBufferGetSize();
	uint32_t size = USART1_SIZE_BUF_TX;
	uint32_t free = USART1_TxBufferGetFreeSpace();
	uint32_t freePercent = (uint32_t)(((free - 64.0) / size) * 100.0);
	
	
	if(freePercent < 10)
	{
		overload = true;
		
		if(!overloadSended)
		{
			static uint8_t dbgBuffer[64];
			static uint16_t dbgBufferLength = 0;
			
			memset(dbgBuffer, 0x00, sizeof(dbgBuffer));
			
			Debug_WriteLine_ToBuffer(dbgBuffer, &dbgBufferLength, "Debug buffer overload !!!!!!!!!!");
			
			USART1_SendBuffer(dbgBuffer, dbgBufferLength);
			
			overloadSended = true;
		}
		
	}
	else if (freePercent > 50)
	{
		overload = false;
		overloadSended = false;
	}
	
	
	if(!overload)
	{
		USART1_SendBuffer(buffer, length);
	}
}

int main(void)
{	
	TIM4_Init();
	TIM4_RegisterInterruptCallback(timer4_Tick);
	TIM4_Start();

	CAN1_Init(CAN1_LS_SPEED_PRESCALE, CAN1_LS_TIME_SEG_1, CAN1_LS_TIME_SEG_2);
	//USB0_Init(); //Configured for loopback //See usb_vcp.c
	
	USART1_Init(Debug_ByteArrived, NULL);
	Debug_Init(DebugSendFunc);
	
#if defined (USE_CAN_HACKER)
	//USART3 Init Inside
	CanHacker_Init();
#endif
	
	// GPIOC Periph clock enable
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// Configure PC13 in output pushpull mode
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOC, &GPIO_InitStructure);



	
	

	
	
	
	
	
	
	/*
	CanRxMsg RxMessage;

	RxMessage.StdId = 0x0180;
	RxMessage.ExtId = 0x00000000;
	RxMessage.IDE = 0x00;
	RxMessage.RTR = 0x00;
	RxMessage.DLC = 0x00;
	RxMessage.Data [0] = 0x46;
	RxMessage.Data [1] = 0x03;
	RxMessage.Data [2] = 0x08;
	RxMessage.Data [3] = 0x07;
	RxMessage.Data [4] = 0xd0;
	RxMessage.Data [5] = 0x6e;
	RxMessage.Data [6] = 0x24;
	RxMessage.Data [7] = 0x00;
	RxMessage.FMI = 0x00;
	
	CanBus_ReceiveMessage(&RxMessage);
	*/
	
	
	
	//Led blinking
	SWtimer_RunTimerCallback(SWTT_Multi_low, 0, 100, pinToggleTimerCb, NULL);
	
	//Debug loop 250ms
#if defined (DEBUG_EN)
	SWtimer_RunTimerCallback(SWTT_Multi_low, 0, 250, RxDebugCmdTimerCb, NULL);
#endif		
	
	
	Debug_WriteLine("Application started.");
	

	// Infinite loop
	while (1)
	{
		CanBus_Run();
		
		SWtimer_TimerTick_LowPriority();
	}
}

/*-- EOF --------------------------------------------------------------------*/

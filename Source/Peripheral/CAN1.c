/*-- File description -------------------------------------------------------*/
/**
*   @file:    CAN1.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "CAN1.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>

/*-- Other libraries --------------------------------------------------------*/
#include "defs.h"

/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"

/*-- Project specific includes ----------------------------------------------*/
#include "bus_worker.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
static void RCC_Periph_Init(void)
{
	// CAN GPIOs configuration
	
	// Enable AFIO
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// Enable PORT
	RCC_APB2PeriphClockCmd(CAN1_PERIPH, ENABLE);
	
	//Enable CAN1
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, ENABLE);
}

static void RCC_Periph_DeInit(void)
{
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1, DISABLE);
}

static void GPIO_Periph_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// CAN RX pin
	GPIO_InitStructure.GPIO_Pin   = CAN1_GPIO_RX;
	//GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);

	// CAN TX pin
	GPIO_InitStructure.GPIO_Pin   = CAN1_GPIO_TX;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);
	
	GPIO_PinRemapConfig(GPIO_Remap1_CAN1, ENABLE);			// Remap Can1 on PB8, PB9
}

static void GPIO_Periph_DeInit(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	// CAN RX and TX pins
	GPIO_InitStructure.GPIO_Pin   = CAN1_GPIO_RX | CAN1_GPIO_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(CAN1_GPIO_PORT, &GPIO_InitStructure);
}

static void NVIC_Periph_Init(void)
{
	
	
    NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	
    NVIC_SetPriority(CAN1_RX1_IRQn, 6);
    NVIC_EnableIRQ(CAN1_RX1_IRQn);
	
	NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 7);
    NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
	
	/*
	// Enable CAN1 RX0 interrupt IRQ channel
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 5);
    NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	*/
}

static void NVIC_Periph_DeInit(void)
{
	//NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
	
    NVIC_DisableIRQ(USB_HP_CAN1_TX_IRQn);
	NVIC_DisableIRQ(CAN1_RX1_IRQn);
	NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn);
}

static void CAN_Periph_Init(uint16_t CAN_Prescaler, uint8_t CAN_BS1, uint8_t CAN_BS2)
{
	// CAN FIFO0 message pending interrupt disable
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);

	//DeInit CAN
    CAN_DeInit(CAN1);
	
	// CAN cell init
	CAN_InitTypeDef CAN_InitStructure;
	CAN_InitStructure.CAN_Prescaler = CAN_Prescaler;
	CAN_InitStructure.CAN_Mode = CAN_Mode_Normal;
	CAN_InitStructure.CAN_SJW = CAN_SJW_1tq;
	CAN_InitStructure.CAN_BS1 = CAN_BS1;
	CAN_InitStructure.CAN_BS2 = CAN_BS2;
	CAN_InitStructure.CAN_TTCM = DISABLE;
	CAN_InitStructure.CAN_ABOM = DISABLE;
	CAN_InitStructure.CAN_AWUM = DISABLE;
	CAN_InitStructure.CAN_NART = DISABLE;
	CAN_InitStructure.CAN_RFLM = DISABLE;
	CAN_InitStructure.CAN_TXFP = DISABLE;	
	
	//Init CAN
	CAN_Init(CAN1, &CAN_InitStructure);

	// CAN FIFO0 message pending interrupt enable
	CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
}

static void CAN_Filter_Init(void)
{
	// CAN filter init
	CAN_FilterInitTypeDef CAN_FilterInitStructure;
	
	CAN_FilterInitStructure.CAN_FilterNumber = 0;
	CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;
	CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;
	CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;
	CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
	CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FIFO0;
	CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;
	
	CAN_FilterInit(&CAN_FilterInitStructure);
}

static void CAN_Periph_DeInit(void)
{
	CAN_ITConfig(CAN1, CAN_IT_FMP0, DISABLE);
	CAN_DeInit(CAN1);
}

/*-- Exported functions -----------------------------------------------------*/
void CAN1_DeInit(void)
{
	CAN_Periph_DeInit();
	NVIC_Periph_DeInit();
	GPIO_Periph_DeInit();
	RCC_Periph_DeInit();
}

void CAN1_Init(uint16_t CAN_Prescaler, uint8_t CAN_BS1, uint8_t CAN_BS2)
{
	CAN1_DeInit();
	
    RCC_Periph_Init();
	GPIO_Periph_Init();
	NVIC_Periph_Init();
	CAN_Periph_Init(CAN_Prescaler, CAN_BS1, CAN_BS2);
	CAN_Filter_Init();
}

uint16_t CAN1_GetPrescaller(void)
{
	uint16_t CAN_Prescaler = 0;
	//uint8_t CAN_Mode = 0;
	//uint8_t CAN_SJW = 0;
	//uint8_t CAN_BS1 = 0;
	//uint8_t CAN_BS2 = 0;
	
	//uint16_t value_get = (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9);
	
	
	CAN_Prescaler = BIT_GET(CAN1->BTR, (BIT0 | BIT1 | BIT2 | BIT3 | BIT4 | BIT5 | BIT6 | BIT7 | BIT8 | BIT9)) + 1;
	//CAN_BS1 = (BIT_GET(CAN1->BTR, (BIT16 | BIT17 | BIT18 | BIT19)) >> 16);
	//CAN_BS2 = (BIT_GET(CAN1->BTR, (BIT20 | BIT21 | BIT22)) >> 20);
	//CAN_SJW = (BIT_GET(CAN1->BTR, (BIT24 | BIT25)) >> 24);
	//CAN_Mode = (BIT_GET(CAN1->BTR, (BIT30 | BIT31)) >> 30);
	
	return CAN_Prescaler;
}

void CAN1_ReInit(uint16_t CAN_Prescaler, uint8_t CAN_BS1, uint8_t CAN_BS2)
{
	CAN1_Init(CAN_Prescaler, CAN_BS1, CAN_BS2);
}

void CAN1_Transmit(CanTxMsg *message)
{
	CAN_Transmit(CAN1, message);
}

void CAN1_InterruptHandler(void)
{
	// CAN Receive Interrupt FIFO 0
	if (CAN_GetITStatus(CAN1, CAN_IT_FMP0) == SET)
	{
		CAN_ClearITPendingBit(CAN1, CAN_IT_FMP0);

		CanRxMsg RxMessage;
		
		// Clear package
		
		RxMessage.StdId = 0x0000;
		RxMessage.ExtId = 0x00000000;
		RxMessage.IDE = 0x00;
		RxMessage.RTR = 0x00;
		RxMessage.DLC = 0x00;
		RxMessage.Data [0] = 0x00;
		RxMessage.Data [1] = 0x00;
		RxMessage.Data [2] = 0x00;
		RxMessage.Data [3] = 0x00;
		RxMessage.Data [4] = 0x00;
		RxMessage.Data [5] = 0x00;
		RxMessage.Data [6] = 0x00;
		RxMessage.Data [7] = 0x00;
		RxMessage.FMI = 0x00;
		
		// Receive data from registers
		CAN_Receive(CAN1, CAN_FIFO0, &RxMessage);
		
		CanBus_ReceiveMessage(&RxMessage);
	}
}

/*-- EOF --------------------------------------------------------------------*/

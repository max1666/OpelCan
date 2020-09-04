/*-- File description -------------------------------------------------------*/
/**
*   @file:    USART1.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/
 
#ifndef USART1_H
#define USART1_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
//USART Interrupt Settings
#define _USART1_Interrupt_Priority      10

//USART RCC Settings
#define _USART1_RCC                     RCC_APB2Periph_GPIOA
#define _USART1_CTS_RCC                 RCC_APB2Periph_GPIOC
#define _USART1_RTS_RCC                 0x00

//USART GPIO Settings
#define _USART1                         USART1
#define _USART1_PORT                    GPIOA

#define _USART1_TX_PORT                 GPIOA
#define _USART1_TX_PIN                  GPIO_Pin_9
#define _USART1_TX_PINN                 GPIO_PinSource9

#define _USART1_RX_PORT                 GPIOA
#define _USART1_RX_PIN                  GPIO_Pin_10
#define _USART1_RX_PINN                 GPIO_PinSource10

#define _USART1_CK_PORT                 0x00
#define _USART1_CK_PIN                  0x00
#define _USART1_CK_PINN                 0x00

#define _USART1_CTS_PORT                GPIOC
#define _USART1_CTS_PIN                 GPIO_Pin_0
#define _USART1_CTS_PINN                GPIO_PinSource0

#define _USART1_RTS_PORT                0x00
#define _USART1_RTS_PIN                 0x00
#define _USART1_RTS_PINN                0x00

//USART Settings
#define _USART1_BaudRate                115200
#define _USART1_WordLength              USART_WordLength_8b
#define _USART1_StopBits                USART_StopBits_1
#define _USART1_Parity                  USART_Parity_No
//Project specific, so it's by 1
#define _1USART1_HardwareFlowControl    USART_HardwareFlowControl_None

//USART DMA Settings
#define USART1_DMA_TX_USE				0
#define USART1_DMA_TX_IT				DMA_ISR_TCIF2
#define USART1_DMA_STREAM_TX            DMA1_Channel2

#define USART1_DMA_RX_USE				0
#define USART1_DMA_RX_IT				DMA_ISR_TCIF3
#define USART1_DMA_STREAM_RX            DMA1_Channel3		

//USART Buffer settings
#define USART1_SIZE_BUF_TX				4096
#define USART1_SIZE_BUF_RX				4096

/*-- Typedefs ---------------------------------------------------------------*/
#if (USART1_DMA_RX_USE == 1)
typedef void (*USART1_RX_CALLBACK_t)(uint8_t *buffer, uint16_t size);
#else
typedef void (*USART1_RX_CALLBACK_t)(uint8_t byte);
#endif // USART1_DMA_RX_USE
typedef void (*USART1_TX_CALLBACK_t)(void);

/*-- Project specific typedefs ----------------------------------------------*/
typedef enum
{
	USART1_MODE_485,
	USART1_MODE_232
}tUSART1_MODE;

/*-- Project specific exported functions ------------------------------------*/
void USART1_SetMode(tUSART1_MODE mode);

/*-- Exported functions -----------------------------------------------------*/
void USART1_Init(USART1_RX_CALLBACK_t callbackRx, USART1_TX_CALLBACK_t callbackTx);
uint8_t USART1_SendBuffer(uint8_t *bufer, uint16_t size);
uint8_t USART1_SendByte(uint8_t byte);

uint16_t USART1_TxBufferGetSize(void);
uint16_t USART1_TxBufferGetFreeSpace(void);
	
void USART1_DMA_InterruptHandler(void);
void USART1_InterruptHandler(void);

#endif // USART1_H
/*-- EOF --------------------------------------------------------------------*/

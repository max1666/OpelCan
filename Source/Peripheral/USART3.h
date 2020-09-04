/*-- File description -------------------------------------------------------*/
/**
*   @file:    USART3.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/
 
#ifndef USART3_H
#define USART3_H
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
//USART Interrupt Settings
#define _USART3_Interrupt_Priority      11     

//USART RCC Settings
#define _USART3_RCC                     RCC_APB2Periph_GPIOB
#define _USART3_CTS_RCC                 0x00
#define _USART3_RTS_RCC                 0x00

//USART GPIO Settings
#define _USART3                         USART3
#define _USART3_PORT                    GPIOB

#define _USART3_TX_PORT                 GPIOB
#define _USART3_TX_PIN                  GPIO_Pin_10
#define _USART3_TX_PINN                 GPIO_PinSource10

#define _USART3_RX_PORT                 GPIOB
#define _USART3_RX_PIN                  GPIO_Pin_11
#define _USART3_RX_PINN                 GPIO_PinSource11

#define _USART3_CK_PORT                 0x00
#define _USART3_CK_PIN                  0x00
#define _USART3_CK_PINN                 0x00

#define _USART3_CTS_PORT                0x00
#define _USART3_CTS_PIN                 0x00
#define _USART3_CTS_PINN                0x00

#define _USART3_RTS_PORT                0x00
#define _USART3_RTS_PIN                 0x00
#define _USART3_RTS_PINN                0x00

//USART Settings
#define _USART3_BaudRate                115200
#define _USART3_WordLength              USART_WordLength_8b
#define _USART3_StopBits                USART_StopBits_1
#define _USART3_Parity                  USART_Parity_No
#define _USART3_HardwareFlowControl     USART_HardwareFlowControl_None

//USART DMA Settings
#define USART3_DMA_TX_USE				0
#define USART3_DMA_TX_IT				DMA_ISR_TCIF2
#define USART3_DMA_STREAM_TX            DMA1_Channel2

#define USART3_DMA_RX_USE				0
#define USART3_DMA_RX_IT				DMA_ISR_TCIF3
#define USART3_DMA_STREAM_RX            DMA1_Channel3		

//USART Buffer settings
#define USART3_SIZE_BUF_TX				384
#define USART3_SIZE_BUF_RX				384

/*-- Typedefs ---------------------------------------------------------------*/
#if (USART3_DMA_RX_USE == 1)
typedef void (*USART3_RX_CALLBACK_t)(uint8_t *buffer, uint16_t size);
#else
typedef void (*USART3_RX_CALLBACK_t)(uint8_t byte);
#endif // USART3_DMA_RX_USE
typedef void (*USART3_TX_CALLBACK_t)(void);

/*-- Exported functions -----------------------------------------------------*/
void USART3_Init(USART3_RX_CALLBACK_t callbackRx, USART3_TX_CALLBACK_t callbackTx);
uint8_t USART3_SendBuf(uint8_t *buf, uint16_t size);
uint8_t USART3_SendByte(uint8_t byte);
	
void USART3_DMA_InterruptHandler(void);
void USART3_InterruptHandler(void);
#endif // USART3_H
/*-- EOF --------------------------------------------------------------------*/

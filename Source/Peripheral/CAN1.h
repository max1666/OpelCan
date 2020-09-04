/*-- File description -------------------------------------------------------*/
/**
*   @file:    CAN1.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

// Note for OBD-II
// PIN3, PIN7, PIN8, PIN12 = Serial Link (ISO9141, KW81, KW82, KWP2000)
// PIN6 - PIN14 = HSCAN - Dual-wire, High Speed CAN-BUS, 500 kbps
// PIN3(H) - PIN11(L) = MSCAN - Dual-wire, Medium Speed CAN-BUS, 95 kbps
// PIN1 = SWCAN - Single-wire, Low Speed CAN-BUS, 33.3 kbps 

#ifndef _CAN_1_H
#define _CAN_1_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_can.h"

/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
#define CAN1_GPIO_PORT					GPIOB
#define CAN1_GPIO_RX                    GPIO_Pin_8              // CRX
#define CAN1_GPIO_TX                    GPIO_Pin_9              // CTX
#define CAN1_PERIPH                     RCC_APB2Periph_GPIOB


#define CAN1_HS_SPEED_PRESCALE             6                       //HS-Can (500 kbps )
#define CAN1_HS_TIME_SEG_1                 CAN_BS1_6tq
#define CAN1_HS_TIME_SEG_2                 CAN_BS2_5tq

#define CAN1_MS_SPEED_PRESCALE             27                      //MS-Can (95 kbps )
#define CAN1_MS_TIME_SEG_1                 CAN_BS1_6tq
#define CAN1_MS_TIME_SEG_2                 CAN_BS2_7tq

#define CAN1_LS_SPEED_PRESCALE             135                     //LS-Can (33.3 kbps )
#define CAN1_LS_TIME_SEG_1                 CAN_BS1_6tq
#define CAN1_LS_TIME_SEG_2                 CAN_BS2_1tq

/*-- Typedefs ---------------------------------------------------------------*/
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void CAN1_Init(uint16_t CAN_Prescaler, uint8_t CAN_BS1, uint8_t CAN_BS2);
void CAN1_DeInit(void);
void CAN1_ReInit(uint16_t CAN_Prescaler, uint8_t CAN_BS1, uint8_t CAN_BS2);
uint16_t CAN1_GetPrescaller(void);
void CAN1_Transmit(CanTxMsg *message);

void CAN1_InterruptHandler(void);

#endif // _CAN_1_H
/*-- EOF --------------------------------------------------------------------*/

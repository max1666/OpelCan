/*-- File description -------------------------------------------------------*/
/**
*   @file:    USART1.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "Usart1.h"
/*-- Standart C/C++ Libraries -----------------------------------------------*/
#include <stdint.h>
#include <string.h>

/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_dma.h"

/*-- Project specific includes ----------------------------------------------*/
#include "RoundBuffer.h"

/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
static uint32_t _USART1_HardwareFlowControl = _1USART1_HardwareFlowControl;
static uint8_t Usart_transferComplete = 0;
static USART1_RX_CALLBACK_t Usart_UserRxCallback = NULL;
static USART1_TX_CALLBACK_t Usart_UserTxCallback = NULL;
#if (USART1_DMA_RX_USE == 1)
static uint8_t Usart_Buffer_Rx[USART1_SIZE_BUF_RX] = {0};
#endif // USART1_DMA_RX_USE

#if (USART1_DMA_TX_USE == 1)
static uint8_t Usart_Buffer_Tx[USART1_SIZE_BUF_TX] = {0};
static uint8_t Usart_DMA_BUSY = 0;
#else
static uint8_t Usart_roundBufferTx[USART1_SIZE_BUF_TX] = {0};
static RoundBuffer_t Usart_Buffer_Tx = {Usart_roundBufferTx, USART1_SIZE_BUF_TX, 0, 0};
#endif // USART1_DMA_TX_USE

/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/******************************************************************************
 *  @brief  USART RCC Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_RCC_Configuration(void)
{	
    /* USART1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);

    /* GPIO clock enable */
    RCC_APB2PeriphClockCmd(_USART1_RCC, ENABLE);

#if ((USART1_DMA_TX_USE == 1) || (USART1_DMA_RX_USE == 1))
    /* DMA1 clock enable */
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
#endif
}

/******************************************************************************
 *  @brief  USART SYSCFG Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_SYSCFG_Configuration(void)
{
	/* SYSCFG DMA Remap*/
	//SYSCFG_DMAChannelRemapConfig(SYSCFG_DMARemap_USART3, ENABLE);
}

/******************************************************************************
 *  @brief  USART GPIO Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_GPIO_Configuration(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //Настройка TX
    GPIO_InitStructure.GPIO_Pin = _USART1_TX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(_USART1_PORT, &GPIO_InitStructure);
	
	//Настройка RX
	GPIO_InitStructure.GPIO_Pin = _USART1_RX_PIN;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(_USART1_PORT, &GPIO_InitStructure);
  
  
    if ((_USART1_HardwareFlowControl == USART_HardwareFlowControl_CTS) || (_USART1_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS))
    {
        //CTS
		RCC_APB2PeriphClockCmd(_USART1_CTS_RCC, ENABLE);

        GPIO_InitStructure.GPIO_Pin = _USART1_CTS_PIN; 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(_USART1_CTS_PORT, &GPIO_InitStructure);
    }
  
    if ((_USART1_HardwareFlowControl == USART_HardwareFlowControl_RTS) || (_USART1_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS))
	{
        //RTS
		RCC_APB2PeriphClockCmd(_USART1_RTS_RCC, ENABLE);
		
        GPIO_InitStructure.GPIO_Pin = _USART1_RTS_PIN; 
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_Init(_USART1_RTS_PORT, &GPIO_InitStructure);
    }
}

/******************************************************************************
 *  @brief  USART Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_USART_Configuration(void)
{
    //Отключить
    USART_DeInit(_USART1);
    USART_Cmd(_USART1, DISABLE);
    
    //Задать параметры
    USART_InitTypeDef USART_InitStructure;
    USART_InitStructure.USART_BaudRate  =       _USART1_BaudRate;
    USART_InitStructure.USART_WordLength =      _USART1_WordLength;
    USART_InitStructure.USART_StopBits =        _USART1_StopBits;
    USART_InitStructure.USART_Parity =          _USART1_Parity;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //CTS && RTS в другом месте отрабатываются программно
    USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

    //Включить
    USART_Init(_USART1, &USART_InitStructure);
    USART_Cmd(_USART1, ENABLE);
}

/******************************************************************************
 *  @brief  USART DMA TX Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_DMA_Tx_Configuration(void)
{
#if (USART1_DMA_TX_USE == 1)
	DMA_InitTypeDef  DMA_InitStructure;
	
    //Заполнить буффер нолями
    memset(Usart_Buffer_Tx, 0, USART1_SIZE_BUF_TX);

    DMA_DeInit(USART1_DMA_STREAM_TX);
 
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart_Buffer_Tx;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&_USART1->DR;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    
    DMA_Init(USART1_DMA_STREAM_TX, &DMA_InitStructure);
	
	/* Enable DMA Stream Transfer Complete interrupt */
    DMA_ITConfig(USART1_DMA_STREAM_TX, DMA_IT_TC, ENABLE);
#endif // USART1_DMA_TX_USE
}

/******************************************************************************
 *  @brief  USART DMA RX Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_DMA_Rx_Configuration(void)
{
#if (USART1_DMA_RX_USE == 1)
    DMA_InitTypeDef  DMA_InitStructure;
	
    DMA_DeInit(USART1_DMA_STREAM_RX);

    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Usart_Buffer_Rx;
    DMA_InitStructure.DMA_BufferSize = (uint16_t)(USART1_SIZE_BUF_RX - 1);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)&_USART1->DR;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;

    DMA_Init(USART1_DMA_STREAM_RX, &DMA_InitStructure);
	
	/* Enable DMA Stream Transfer Complete interrupt */
	//DMA_ITConfig(USART1_DMA_STREAM_RX, DMA_IT_TC, ENABLE);
  
	/* Enable the DMA RX Stream */
    DMA_Cmd(USART1_DMA_STREAM_RX, ENABLE);
	
	/* Enable USART for DMA */
	USART_DMACmd(_USART1, USART_DMAReq_Rx, ENABLE);
#endif // USART1_DMA_RX_USE
}

/******************************************************************************
 *  @brief  USART NVIC Configuration function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_NVIC_Configuration(void)
{
    NVIC_SetPriority(USART1_IRQn, _USART1_Interrupt_Priority);
    NVIC_EnableIRQ(USART1_IRQn);
	
#if (USART1_DMA_RX_USE == 1)
    USART_ITConfig(_USART1, USART_IT_IDLE, ENABLE);
#else
	USART_ITConfig(_USART1, USART_IT_RXNE, ENABLE);
#endif // (USART1_DMA_RX_USE == 1)
    USART_ITConfig(_USART1, USART_IT_TC, ENABLE); //Прерывание на окончание передачи
  
#if ((USART1_DMA_TX_USE || USART1_DMA_RX_USE) == 1)
    NVIC_SetPriority(DMA1_Channel2_3_IRQn, 0);
    NVIC_EnableIRQ(DMA1_Channel2_3_IRQn);
#endif // ((USART1_DMA_TX_USE || USART1_DMA_RX_USE) == 1)
}

/******************************************************************************
 *  @brief  USART Send data from buffer by interrupt function
 *
 *  @retval None.
 *****************************************************************************/
static void USART_SendBuf_IT(void)
{
#if (USART1_DMA_TX_USE == 0)
	if(RoundBuffer_GetLoad(&Usart_Buffer_Tx) > 0)
	{
		uint8_t byte = RoundBuffer_GetByte(&Usart_Buffer_Tx);
		
		//Прерывание по началу передачи
		USART_ITConfig(_USART1, USART_IT_TXE, ENABLE);
		
		USART_SendData(_USART1, byte);
	}
	else
	{
		Usart_transferComplete = 1;
		
		if(Usart_UserTxCallback != NULL)
		{
			Usart_UserTxCallback();
		}
	}
#endif // USART1_DMA_TX_USE
}

static void USART1_CallBack(uint8_t byte, uint8_t *buffer, uint16_t size)
{
	if(Usart_UserRxCallback)
	{
#if (USART1_DMA_RX_USE == 1)
		Usart_UserRxCallback(Usart_Buffer_Rx, size);
#else
		Usart_UserRxCallback(byte);
#endif // USART1_DMA_RX_USE
	}
}

//Project specific!
/******************************************************************************
 *  @brief  USART Set working moide function
 *
 *  @arg    mode - Working mode.
 *
 *  @retval None.
 *****************************************************************************/
void USART1_SetMode(tUSART1_MODE mode)
{
	if (mode == USART1_MODE_232)
	{
		_USART1_HardwareFlowControl = USART_HardwareFlowControl_None;
		GPIO_SetBits(_USART1_CTS_PORT, _USART1_CTS_PIN);
		//GPIO_ResetBits(_USART1_CTS_PORT, _USART1_CTS_PIN);
	}
	else
	{
		//GPIO_SetBits(_USART1_CTS_PORT, _USART1_CTS_PIN);
		GPIO_ResetBits(_USART1_CTS_PORT, _USART1_CTS_PIN);
		_USART1_HardwareFlowControl = USART_HardwareFlowControl_CTS;
	}
}

/******************************************************************************
 *  @brief  USART Initializing function
 *
 *  @arg    callback - User's callback when data arrived.
 *
 *  @retval None.
 *****************************************************************************/
void USART1_Init(USART1_RX_CALLBACK_t callbackRx, USART1_TX_CALLBACK_t callbackTx)
{
    USART_RCC_Configuration();
	USART_SYSCFG_Configuration();
    USART_GPIO_Configuration();
	
	USART1_SetMode(USART1_MODE_232);
	
    USART_USART_Configuration();
    USART_DMA_Tx_Configuration();
    USART_DMA_Rx_Configuration();
    USART_NVIC_Configuration();

#if (USART1_DMA_TX_USE == 1)	
	Usart1_DMA_BUSY = 0;
#endif // USART1_DMA_TX_USE
	
	Usart_UserRxCallback = callbackRx;
	Usart_UserTxCallback = callbackTx;
}

/******************************************************************************
 *  @brief  USART Initializing function
 *
 *  @arg    buf - pointer to data to send.
 *  @arg    size - sending data size.
 *
 *  @retval 1 - when OK, 0 - when FAILED.
 *****************************************************************************/
uint8_t USART1_SendBuffer(uint8_t *bufer, uint16_t size)
{
    if(!size || !bufer)
    {
        return 0;
    }
	
#if (USART1_DMA_TX_USE == 1)
	if(Usart1_DMA_BUSY)
	{
		return 0;
	}
#endif // USART1_DMA_TX_USE
	
    //Засунем данные в буффер передачи
	RoundBuffer_AddArray(&Usart_Buffer_Tx, bufer, size);
  
#if (USART1_DMA_TX_USE == 1)
	//Прерывание по началу передачи
    USART_ITConfig(_USART1, USART_IT_TXE, ENABLE);
	
	USART1_DMA_STREAM_TX->CNDTR = size;
	Usart_DMA_BUSY = 1;
	DMA_Cmd(USART1_DMA_STREAM_TX, ENABLE);
	USART_DMACmd(_USART1, USART_DMAReq_Tx, ENABLE);
#else
	if(Usart_transferComplete)
	{
		Usart_transferComplete = 0;
		USART_SendBuf_IT();
	}
#endif // USART1_DMA_TX_USE
    return 1;
}

/******************************************************************************
 *  @brief  USART Send byte function
 *
 *  @arg    byte - byte to send
 *
 *  @retval 1 - when OK, 0 - when FAILED.
 *****************************************************************************/
uint8_t USART1_SendByte(uint8_t byte)
{
	return USART1_SendBuffer(&byte, 1);
}

uint16_t USART1_TxBufferGetSize(void)
{
	return (uint16_t)RoundBuffer_GetSize(&Usart_Buffer_Tx);
}

uint16_t USART1_TxBufferGetFreeSpace(void)
{
	return (uint16_t)RoundBuffer_GetFree(&Usart_Buffer_Tx);
}

/******************************************************************************
 *  @brief  USART DMA Interrupt handler function
 * 			Should be called from DMA Chanel interrupt
 *
 *  @arg    callback - User's callback when data arrived.
 *
 *  @retval None.
 *****************************************************************************/
void USART1_DMA_InterruptHandler(void) // USART1_TX
{
	/* Test on DMA Stream Transfer Complete interrupt */
    if (DMA_GetITStatus(USART1_DMA_TX_IT))
    {
        /* Clear DMA Stream Transfer Complete interrupt pending bit */
        DMA_ClearITPendingBit(USART1_DMA_TX_IT);
    
#if (USART1_DMA_TX_USE == 1)	

        if(!Usart1_DMA_BUSY)
        {
            return;
        }
    
        DMA_Cmd(USART1_DMA_STREAM_TX, DISABLE);

        Usart1_DMA_BUSY = 0;
#endif // USART1_DMA_TX_USE
		
    }
	
    /* Test on DMA Stream Transfer Complete interrupt */
    if (DMA_GetITStatus(USART1_DMA_RX_IT))
    {
        /* Clear DMA Stream Transfer Complete interrupt pending bit */
        DMA_ClearITPendingBit(USART1_DMA_RX_IT);
    }
}

/******************************************************************************
 *  @brief  USART Interrupt handler function
 * 			Should be called from USART interrupt
 *
 *  @arg    callback - User's callback when data arrived.
 *
 *  @retval None.
 *****************************************************************************/
void USART1_InterruptHandler(void)
{
	
    //Прерывание по завершению ПРИЕМА фрейма данных
    if(USART_GetITStatus(_USART1, USART_IT_IDLE) != RESET)
    {
		USART_ClearITPendingBit(_USART1, USART_IT_IDLE);
		
#if (USART1_DMA_RX_USE == 1)
		
		DMA_Cmd(USART1_DMA_STREAM_RX, DISABLE);
	
		//Если размер больше 0
		if ((USART1_SIZE_BUF_RX - 1) - USART1_DMA_STREAM_RX->CNDTR > 0)
		{
			//Кидаем коллбек
			USART1_CallBack(0x00, Usart1_Buffer_Rx, ((USART1_SIZE_BUF_RX - 1) - USART1_DMA_STREAM_RX->CNDTR));
		}
		
        USART1_DMA_STREAM_RX->CNDTR = USART1_SIZE_BUF_RX - 1;
   
        //!!!! По неизвестной причине после STREM_RX DISABLE TCIF устаноавливается в 1
        // если его не сбросить DMA по этому потоку не работаeт
        DMA_ClearITPendingBit(USART1_DMA_RX_IT);
		
        DMA_Cmd(USART1_DMA_STREAM_RX, ENABLE);
#endif // USART1_DMA_RX_USE
    }
	
	
    //Прерывание по НАЧАЛУ ПЕРЕДАЧИ данных
    if(USART_GetITStatus(_USART1, USART_IT_TXE) != RESET)
    {   
		USART_ClearITPendingBit(_USART1, USART_IT_TXE);
        
        USART_ITConfig(_USART1, USART_IT_TXE, DISABLE);
		
        //Програмная обработка CTS
        if ((_USART1_HardwareFlowControl == USART_HardwareFlowControl_CTS) || (_USART1_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS))
        {
            //CTS ON
            GPIO_SetBits(_USART1_CTS_PORT, _USART1_CTS_PIN);
        }
    }
   
   
    //Прерывание по ЗАВЕРШЕНИЮ ПЕРЕДАЧИ
    if(USART_GetITStatus(_USART1, USART_IT_TC) != RESET)
    {   
		USART_ClearITPendingBit(_USART1, USART_IT_TC);
		
		//Програмная обработка CTS
        if ((_USART1_HardwareFlowControl == USART_HardwareFlowControl_CTS) || (_USART1_HardwareFlowControl == USART_HardwareFlowControl_RTS_CTS))
        {
            //CTS OFF
            GPIO_ResetBits(_USART1_CTS_PORT, _USART1_CTS_PIN);
        }

		USART_SendBuf_IT();
    }
	
	
	//Прерывание по НАЛИЧИЮ ДАННЫХ В РЕГИСТРЕ ПРИЕМА
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		USART_ClearITPendingBit(_USART1, USART_IT_RXNE);
		
		USART1_CallBack(USART_ReceiveData(_USART1), NULL, 0x01);
	}
	
	//Прерывание по Переполнение буффера приема
	if(USART_GetFlagStatus(USART1, USART_FLAG_ORE) != RESET)
	{
		USART_ClearITPendingBit(_USART1, USART_IT_ORE);
	}
}

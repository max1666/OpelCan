/*-- File description -------------------------------------------------------*/
/**
*   @file:    main.h
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#ifndef __MAIN_H
#define __MAIN_H
/*-- Standard C/C++ Libraries -----------------------------------------------*/
/*-- Other libraries --------------------------------------------------------*/
/*-- Hardware specific libraries --------------------------------------------*/
/*-- Project specific includes ----------------------------------------------*/
/*-- Exported macro ---------------------------------------------------------*/
/*-- Typedefs ---------------------------------------------------------------*/
typedef enum
{
	APP_STATE__START,
	APP_STATE__WAIT_START,
	APP_STATE__INIT,
	APP_STATE__WAIT_INIT,
	APP_STATE__WORK
} AppState_t;
/*-- Exported variables -----------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/

#endif // __MAIN_H
/*-- EOF --------------------------------------------------------------------*/

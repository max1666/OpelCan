/*-- File description -------------------------------------------------------*/
/**
*   @file:    common.c
*
*   @author:  valeriy.williams.
*   @company: Lab.
*/

#include "common.h"

/*-- Standard C/C++ Libraries -----------------------------------------------*/
#include <string.h>
/*-- Other libraries --------------------------------------------------------*/
#include "debug.h"
/*-- Hardware specific libraries --------------------------------------------*/
#include "stm32f10x.h"
/*-- Project specific includes ----------------------------------------------*/
/*-- Imported functions -----------------------------------------------------*/
/*-- Local Macro Definitions ------------------------------------------------*/
/*-- Local Typedefs ---------------------------------------------------------*/
/*-- Local variables --------------------------------------------------------*/
/*-- Local function prototypes ----------------------------------------------*/
/*-- Local functions --------------------------------------------------------*/
/*-- Exported functions -----------------------------------------------------*/
void System_Reset(void)
{
	Debug_WriteLine("");
	Debug_WriteLine("");
	Debug_WriteLine("-------------------------------");
	Debug_WriteLine("");
	Debug_WriteLine("   Going down to reset NOW !  ");
	Debug_WriteLine("");
	Debug_WriteLine("-------------------------------");
	Debug_WriteLine("");
	Debug_WriteLine("");

	NVIC_SystemReset();
}

/******************************************************************************
* Function:      RIL_FindString
*
* Description:
*                This function is used to match string within a specified length.
*                This function is very much like strstr.
*
* Parameters:
*                line:
*                    [in]The address of the string.
*                len:
*                    [in]The length of the string.
*                str:
*                    [in]The specified item which you want to look for in the string.
*
* Return:
                The function returns a pointer to the located string,
                or a  null  pointer  if  the specified string is not found.
******************************************************************************/
char *RIL_FindString(char *line, uint32_t len, char *str)
{
    int32_t i;
    int32_t v_strLen;
    char *p;

    if ((NULL == line) || (NULL == str))
	{
        return NULL;
	}

    v_strLen = strlen(str);
    if(v_strLen > len)
    {
        return NULL;
    }

    p = line;
    for (i = 0;i < len - v_strLen + 1; i++)
    {
        if (0 == strncmp (p, str, v_strLen))
        {
            return p;
        }
		else
		{
            p++;
        }
    }
    return NULL;
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
uint32_t HexCharStrToInt(char *hex)
{
    uint32_t val = 0;

    while (*hex)
    {
        // get current character then increment
        uint8_t byte = *hex++;

        // transform hex character to the 4bit equivalent number, using the ascii table indexes
        if (byte >= '0' && byte <= '9')
        {
        	byte = byte - '0';
        }
        else if (byte >= 'a' && byte <='f')
        {
        	byte = byte - 'a' + 10;
        }
        else if (byte >= 'A' && byte <='F')
        {
        	byte = byte - 'A' + 10;
        }
        else
        {
        	break;
        }

        // shift 4 to make space for new digit, and add the 4 bits of the new digit
        val = (val << 4) | (byte & 0xF);
    }

    return val;
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
int32_t DecCharStrToInt(char *dec)
{
    int32_t result = 0;
    int32_t puiss = 1;

	while (('-' == (*dec)) || ((*dec) == '+'))
	{
		if (*dec == '-')
		{
			puiss = puiss * -1;
		}

		dec++;
	}

	while ((*dec >= '0') && (*dec <= '9'))
	{
		result = (result * 10) + ((*dec) - '0');

		dec++;
	}

	return (result * puiss);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
uint32_t DecCharStrToUint(char *dec)
{
    uint32_t result = 0;
    int32_t puiss = 1;

	while (('-' == (*dec)) || ((*dec) == '+'))
	{
		if (*dec == '-')
		{
			puiss = puiss * -1;
		}

		dec++;
	}

	while ((*dec >= '0') && (*dec <= '9'))
	{
		result = (result * 10) + ((*dec) - '0');

		dec++;
	}

	return (uint32_t)(result * puiss);
}

/******************************************************************************
 *  @brief  Function
 *
 *  @param  None.
 *
 *  @retval None.
 *****************************************************************************/
int32_t PowFast(int32_t num, int32_t deg)
{
	int32_t result = 1;

    if(deg < 0)
    {
        deg = -deg;

        while(deg)
        {
            if (deg % 2 == 0)
            {
                deg /= 2;
                num *= num;
            }
            else
            {
                deg--;
                result *= num;
            }
        }

        return 1 / result;
    }
    else
    {
        while(deg)
        {
            if (deg % 2 == 0)
            {
                deg /= 2;
                num *= num;
            }
            else
            {
                deg--;
                result *= num;
            }
        }

        return result;
    }
}

/*-- EOF --------------------------------------------------------------------*/

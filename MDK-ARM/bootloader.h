/**
 ******************************************************************************
 * @file           : bootloader.h
 * @author         : Romany Sobhy
 ******************************************************************************
 */

#ifndef BOOTLOADER_H
#define BOOTLOADER_H


/*------------------ INCLUDES START -------------------------------------*/
#include "usart.h"
#include <string.h>
#include <stdarg.h>


/*------------------ INCLUDES END --------------------------------------*/



/*------------------ MACRO DECLARATION ----------------------*/
#define BL_DEBUG_UART &huart2

/*------------------ MACRO FUNCTIONS END ---------------------*/



/*------------------ MACRO FUNCTIONS DECLARATION ----------------------*/


/*------------------ MACRO FUNCTIONS DECLARATION END -----------------*/


/*------------------ DATA TYPE DECLARATIONS --------------------------*/
typedef enum {
		BL_NACK=0,
		BL_OK

}BL_status;


/*------------------ DATA TYPE DECLARATIONS END ---------------------*/


/*------------------ SW INTERFACES DECLARATIONS ---------------------*/
void BL_Print_Message(char *format, ...);
BL_status BL_UART_FETCH_HOST_COMMAND(void);

/*------------------ SW INTERFACES DECLARATIONS END -----------------*/


#endif /*BOOTLOADER_H*/

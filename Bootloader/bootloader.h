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
#include <stdio.h>
#include "crc.h"

/*------------------ INCLUDES END --------------------------------------*/



/*------------------ MACRO DECLARATION ----------------------*/
#define BL_DEBUG_UART                &huart2
#define BL_HOST_COMMUNICATION_UART   &huart2 

#define CRC_Engine_Obj               &hcrc


#define	 DEBUG_INFO_ENABLE						1
#define	 DEBUG_INFO_DISABLE						0
#define  BL_DEBUG_ENABLE						DEBUG_INFO_ENABLE


#define DEBUG_METHOD_UART            0x00
#define DEBUG_METHOD_SPI             0x01
#define DEBUG_METHOD_CAN             0x02
#define DEBUG_METHOD                 DEBUG_METHOD_UART
#define BL_HOST_BUFFER_length        200



#define CBL_GET_VER_CMD												0x10
#define CBL_GET_HELP_CMD											0x11
#define CBL_GET_CID_CMD												0x12
#define CBL_GET_RDP_STATUS_CMD								0x13
#define CBL_GO_TO_ADDR_CMD										0x14
#define CBL_FLASH_ERASE_CMD										0x15
#define CBL_MEM_WRITE_CMD											0x16
#define CBL_EN_R_W_PROTECT_CMD								0x17
#define CBL_MEM_READ_CMD											0x18
#define CBL_READ_SECTOR_STATUS_CMD						0x19
#define CBL_OTP_READ_CMD											0x20
#define CBL_CHANGE_ROP_LEVEL_CMD							0x21


/**************************** BL Version**************************/
                     	   
#define CBL_VERSION_ID											  100
#define CBL_SW_MAJOR_VERSION								  1
#define CBL_SW_MINORR_VERSION								  0
#define CBL_SW_PATCH_VERSION								  0




/*******************************************************************************
*                        		CRC	 		                                  		 *
*******************************************************************************/
#define CRC_OK															  1
#define CRC_NOK															  0
#define CRC_TYPE_SIZE                         4  

#define CBL_SEND_ACK                          0xAB
#define CBL_SEND_NACK                         0xCD


/* Start address of sector 2 */
#define FLASH_SECTOR2_BASE_ADDRESS             0x8008000U



#define ADDRESS_VALID 												0x01
#define ADDRESS_NOT_VALID 										0x00

#define STM32F103_SRAM_BASE						       (0x20000000)
#define STM32F103_SRAM_END									 (STM32F103_SRAM_BASE+(20*1024))
#define STM32F103_FLASH_BASE								 (0x08000000)
#define STM32F103_FLASH_END									 (STM32F103_FLASH_BASE+(64*1024))
#define APP_BASE_ADDREESS										  0x08008000



/**Erase Function**/
#define INVALID_SECTOR_NUMBER                 0X00
#define VALID_SECTOR_NUMBER                   0X01

#define UNSUCCESSFUL_ERASE                    0X02
#define SUCCESSFUL_ERASE                      0X03
#define HAL_SUCCESSFUL_ERASE                  0xFFFFFFFFU
/*ASSUME (low-density devices) */
#define CBL_MAX_PAGE_NUMBER								    32
#define CBL_Mass_ERASE						      		  0xFF

/* CBL_GET_RDP_STATUS_CMD	*/
#define RDP_LEVEL_READ_INVALID                0x00
#define RDP_LEVEL_READ_VALID                  0x01

/**************************** CBL_MEM_WRITE_CMD**************************/

#define FLASH_PAYLOAD_WRITE_FAILED           0X00
#define FLASH_PAYLOAD_WRITE_PASSED           0X01

#define FLASH_LOCK_FAILED           0X00
#define FLASH_LOCK_PASSED           0X01
/*------------------ MACRO FUNCTIONS END ---------------------*/

void BL_Print_Message(char *format, ...);

/*------------------ MACRO FUNCTIONS DECLARATION ----------------------*/


/*------------------ MACRO FUNCTIONS DECLARATION END -----------------*/


/*------------------ DATA TYPE DECLARATIONS --------------------------*/
typedef enum {
		BL_NACK=0,
		BL_OK

}BL_status;

/**x**/typedef void(*pMainApp)(void);

typedef  void  (*Jump_Ptr)(void);
/*------------------ DATA TYPE DECLARATIONS END ---------------------*/


/*------------------ SW INTERFACES DECLARATIONS ---------------------*/
/**
 * @brief Prints a formatted message to the output.
 *
 * @param format The format string for the message.
 * @param ... Additional arguments for the format string.
 */
void BL_Print_Message(char *format, ...);

/**
 * @brief Fetches a command from the host via UART.
 *
 * @return The status of the command fetch operation.
 */
BL_status BL_UART_FETCH_HOST_COMMAND(void);







/*------------------ SW INTERFACES DECLARATIONS END -----------------*/


#endif /*BOOTLOADER_H*/

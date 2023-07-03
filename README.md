 # STM32F103C8_Basic_Bootloader
STM32F103C8 Bootloader: Simple firmware updater via UART. Supports external programming and customizable command protocol.

## Tools used:
1- STTM32 CubeMX
2- Keil uVision5 IDE
3- HxD
4- STM32 ST-LINK Utility
5- Python Host Script
 ## Features:
- Bootloader code that runs on the STM32F103C8 microcontroller.
- Supports firmware updates via UART communication.
- Provides a simple command protocol for firmware programming.
- Allows firmware updates from an external source, such as a computer or another microcontroller.

 ## BL Usage: 
1. Connect the UART interface of the STM32F103C8 to your programming device (e.g., computer or another microcontroller).
2. Run the provided firmware update tool on the programming device.
3. Use the tool to send firmware update commands to the bootloader via UART.
4. The bootloader will receive and program the new firmware into the microcontroller's flash memory.
5. After programming, reset the microcontroller to start running the updated firmware.

Please note that this is a basic bootloader implementation and may require modifications or enhancements depending on your specific requirements. Feel free to customize and extend it to meet your needs.

 ## Contributing:
Contributions to this project are welcome. If you find any issues or have suggestions for improvements, please open an issue or submit a pull request.

 ## Commands and Usage
STM32F103 Custom Bootloader
==============================

Which command you need to send to the bootloader:

1. `CBL_GET_VER_CMD` --> 1
2. `CBL_GET_HELP_CMD` --> 2
3. `CBL_GET_CID_CMD` --> 3
4. `CBL_GET_RDP_STATUS_CMD` --> 4
5. `CBL_GO_TO_ADDR_CMD` --> 5
6. `CBL_FLASH_ERASE_CMD` --> 6
7. `CBL_MEM_WRITE_CMD` --> 7
8. `CBL_ED_W_PROTECT_CMD` --> 8
9. `CBL_MEM_READ_CMD` --> 9
10. `CBL_READ_SECTOR_STATUS_CMD` --> 10
11. `CBL_OTP_READ_CMD` --> 11
12. `CBL_CHANGE_ROP_Level_CMD` --> 12

Implemented Functions:
----------------------
STM32F103 Custom Bootloader
==============================

Python Host commands:

1. `CBL_GET_VER_CMD` --> 1
2. `CBL_GET_HELP_CMD` --> 2
3. `CBL_GET_CID_CMD` --> 3
4. `CBL_GET_RDP_STATUS_CMD` --> 4
5. `CBL_GO_TO_ADDR_CMD` --> 5
6. `CBL_FLASH_ERASE_CMD` --> 6
7. `CBL_MEM_WRITE_CMD` --> 7
8. `CBL_ED_W_PROTECT_CMD` --> 8
9. `CBL_MEM_READ_CMD` --> 9
10. `CBL_READ_SECTOR_STATUS_CMD` --> 10
11. `CBL_OTP_READ_CMD` --> 11
12. `CBL_CHANGE_ROP_Level_CMD` --> 12

Implemented Functions:
----------------------

 ### 1. Command: CBL_GET_VER_CMD
   Description: Retrieves the version information of the bootloader. Sends the bootloader version as a response. This command is used to determine the version of the bootloader currently running on the microcontroller.

 ### 2. Command: CBL_GET_HELP_CMD
   Description: Provides a list of available commands and their descriptions. Sends the command list and descriptions as a response. This command is helpful for users to understand the available functionality and usage of various commands supported by the bootloader.

 ### 3. Command: CBL_GET_CID_CMD
   Description: Retrieves the unique chip ID of the microcontroller. Sends the chip ID as a response. The chip ID is a unique identifier associated with each microcontroller and can be used for identification or verification purposes, which is 0x410 in case of this MCU

 ### 4. Command: CBL_GET_RDP_STATUS_CMD
   Description: Retrieves the Read Protection (RDP) status of the microcontroller. Sends the RDP status as a response. The RDP status indicates whether the flash memory of the microcontroller is protected against read operations. This command allows users to check the current protection status, BL will return value 0 or 1.

 ### 5. Command: CBL_GO_TO_ADDR_CMD
 Description: Allows the host to specify an address to which the bootloader should jump. The host prompts the user for the desired address and sends the command to the bootloader for execution. If the provided address is 0x08008000, It's important to note that if an invalid address is given, the bootloader will refuse to jump to that address and respond with a NACK (Negative Acknowledgement) to indicate the failure.

 ### 6. Command:CBL_FLASH_ERASE_CMD

Description:
This command is responsible for erasing flash memory sectors in the microcontroller. The implementation includes the Perform_Flash_Erase function and the handleCBL_FLASH_ERASE_CMD function.
The Perform_Flash_Erase function performs the flash erase operation based on the provided parameters, which are the starting address of the sector to be erased (PageAddr) and the number of sectors to be erased (Nb_Pages). It returns a status indicating the success or failure of the erase operation.
The handleCBL_FLASH_ERASE_CMD function is triggered when the bootloader receives the flash erase command from the host. It verifies the integrity of the command packet using CRC (Cyclic Redundancy Check). If the CRC verification is successful, an acknowledgment (ACK) is sent to the host.

The function then calls the Perform_Flash_Erase function with the specified page address and number of pages to perform the flash erase operation. The erase status returned by the Perform_Flash_Erase function is transmitted back to the host.

If the flash erase operation is successful, the success status is transmitted to the host. Otherwise, the failure status is sent. Debug messages can be printed if the debug mode is enabled.

Note: The maximum number of flash pages (CBL_MAX_PAGE_NUMBER) is defined as 32 assuming low_density_devices


 ### Command 7: FLASH_MEM_WRITE_PAYLOAD

Description:
This command writes payload data to the flash memory of the microcontroller. The FLASH_MEM_WRITE_PAYLOAD function performs the write operation, taking the host payload, payload start address, and payload length as inputs. It returns a status indicating the success or failure of the write operation.

The function unlocks the flash memory and iterates through the payload data, writing each byte to the corresponding flash memory address. If any write operation fails, the status is set to "Failed." After writing the payload, the flash memory is locked again.

The handleCBL_MEM_WRITE_CMD function is called upon receiving the memory write command. It verifies the command packet's integrity using CRC and sends an acknowledgment (ACK) to the host. The function extracts the payload address and length, verifies the address, and calls FLASH_MEM_WRITE_PAYLOAD to write the payload data. The status is then transmitted back to the host.

Note: Application bin  is to be written in 0x8008000 address.


 ### command:8->11 to be implemented in future updates.

 ### Command 12: CBL_CHANGE_ROP_Level_CMD 
In the case of the stm32f103 MCU, there are two available levels for the flash read protection:

Level 0: This is the default level, allowing normal read access to the flash memory.
Level 1: This level disables read access to the flash memory. It is important to note that when using level 1 protection, debugging the MCU becomes impossible as it triggers the HardFault handler. To enable debugging and read access to the flash memory, it is necessary to revert back to level 0. However, changing the protection level from 1 to 0 will erase the entire chip, requiring the re-uploading of the code.




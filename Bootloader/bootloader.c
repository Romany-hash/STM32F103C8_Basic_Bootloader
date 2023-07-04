/**
 ******************************************************************************
 * @file           : bootloader.c
 * @author         : Romany Sobhy
 ******************************************************************************
 */


/*------------------ INCLUDES START -------------------------------------*/
#include "bootloader.h"

/*------------------ INCLUDES END --------------------------------------*/

/* ------------------------------GLOBAL VAR DECLERATIONS----------------------------*/
static uint8_t BL_HOST_BUFFER[BL_HOST_BUFFER_length];

static uint8_t BL_Supported_CMDs[12] = {
    CBL_GET_VER_CMD,
    CBL_GET_HELP_CMD,
    CBL_GET_CID_CMD,
    CBL_GET_RDP_STATUS_CMD,
    CBL_GO_TO_ADDR_CMD,
    CBL_FLASH_ERASE_CMD,
    CBL_MEM_WRITE_CMD,
    CBL_EN_R_W_PROTECT_CMD,
    CBL_MEM_READ_CMD,
    CBL_READ_SECTOR_STATUS_CMD,
    CBL_OTP_READ_CMD,
    CBL_CHANGE_ROP_LEVEL_CMD
};

/*------------------ MACRO DECLARATION ----------------------*/


/*------------------ MACRO FUNCTIONS END ---------------------*/

/**
 * @brief Handles the CBL_GET_VER_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_GET_VER_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_GET_HELP_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_GET_HELP_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_GET_CID_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_GET_CID_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_GET_RDP_STATUS_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_GET_RDP_STATUS_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_GO_TO_ADDR_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_GO_TO_ADDR_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_FLASH_ERASE_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_FLASH_ERASE_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_MEM_WRITE_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_MEM_WRITE_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_EN_R_W_PROTECT_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_EN_R_W_PROTECT_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_MEM_READ_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_MEM_READ_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_READ_SECTOR_STATUS_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_READ_SECTOR_STATUS_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_OTP_READ_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */
static void handleCBL_OTP_READ_CMD(uint8_t* BL_HOST_BUFFER);

/**
 * @brief Handles the CBL_CHANGE_ROP_LEVEL_CMD command.
 *
 * @param BL_HOST_BUFFER The buffer containing the command data.
 */


/**
 * @brief Verifies the CRC of the data received from the host.
 *
 * @param pType      Pointer to the type of data being verified.
 * @param Data_Len   Length of the data to verify.
 * @param Host_CRC   CRC value sent by the host.
 *
 * @return 1 if the CRC is verified successfully, 0 otherwise.
 */
static uint8_t Bootloader_CRC_verify(uint8_t* pType, uint32_t Data_Len, uint32_t Host_CRC);


#include <stdint.h>

/**
 * @brief Sends an ACK (Acknowledgement) message.
 *
 * @param Replay_Length Length of the reply message.
 */
static void BL_Send_ACK(uint8_t Replay_Length);

/**
 * @brief Sends a NACK (Negative Acknowledgement) message.
 */
static void BL_Send_NACK();


/**
 * @brief Jumps to the user application.
 *
 * @note This function assumes that the user application is properly flashed and located at the correct
 *       address specified in the linker script.
 */
static void bootloader_Jump_to_User_App(void);


/********implemntation********/




/*------------------ MACRO FUNCTIONS DECLARATION ----------------------*/


/*------------------ MACRO FUNCTIONS DECLARATION END -----------------*/


/*------------------ DATA TYPE DECLARATIONS --------------------------*/


/*------------------ DATA TYPE DECLARATIONS END ---------------------*/


/*------------------ Functions Definitions ---------------------*/


static uint8_t Bootloader_CRC_verify(uint8_t *pData, uint32_t Data_Len, uint32_t Host_CRC){
				
		uint8_t  CRC_STATUS         = CRC_NOK;
		uint32_t MCU_CRC_Calculated = 0;
		uint32_t Data_Buffer        = 0;
		uint8_t Data_Counter        = 0;
	/* Calculate CRC32 */
	for(Data_Counter=0;Data_Counter<Data_Len;Data_Counter++) {
		Data_Buffer = ((uint32_t)pData[Data_Counter]);
		MCU_CRC_Calculated = HAL_CRC_Accumulate(CRC_Engine_Obj, &Data_Buffer, 1 );
	}
	
	__HAL_CRC_DR_RESET(CRC_Engine_Obj);
	
	if(MCU_CRC_Calculated==CRC_STATUS) {
			CRC_STATUS=CRC_OK;
		}
		
	else {

				CRC_STATUS=CRC_NOK;
		}	
	
	return CRC_STATUS;
	


}

static void BL_Send_ACK(uint8_t Replay_Length){
	uint8_t Ack_Value[2] = {0};
	Ack_Value[0]=CBL_SEND_ACK;
	Ack_Value[1]=Replay_Length;
	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)Ack_Value, 2, HAL_MAX_DELAY);
}

static void BL_Send_NACK(){
		uint8_t Ack_Value = CBL_SEND_NACK;
			HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, &Ack_Value, 1, HAL_MAX_DELAY);
}



static void handleCBL_GET_VER_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_GET_VER_CMD
   uint8_t BL_VERSION[4]= {CBL_VERSION_ID	,CBL_SW_MAJOR_VERSION,CBL_SW_MINORR_VERSION, CBL_SW_PATCH_VERSION	};
		uint16_t Host_CMD_Packet_Len=0;
	 uint32_t  Host_CRC32=0;

	 #if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("Read the Bootloader version from the MCU \r\n");
	 #endif
	 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));

  /*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
		 BL_Send_ACK(4);
			HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)BL_VERSION, 4, HAL_MAX_DELAY);
	}
	else {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Failed\r\n");
	 #endif
		BL_Send_NACK();
	}
}

static void handleCBL_GET_HELP_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_GET_HELP_CMD
    uint16_t Host_CMD_Packet_Len=0;
	 uint32_t  Host_CRC32=0;
	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("CBL_GET_HELP_CMD reached.\r\n");
	 #endif
	
	 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));


/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
	 BL_Send_ACK(12);
			HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)BL_Supported_CMDs, 12, HAL_MAX_DELAY);
	 }	 
			
	
else {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Failed\r\n");
	 #endif
		BL_Send_NACK();

	}
}


static void handleCBL_GET_CID_CMD(uint8_t* BL_HOST_BUFFER) {
	// Implementation for CBL_GET_CID_CMD
     uint16_t Host_CMD_Packet_Len   =0;
	   uint32_t  Host_CRC32           =0;
	   uint16_t MCU_ID_NO             =0;
	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CBL_GET_CID_CMD reached.\r\n");
	 #endif
	
	 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));

	
	
/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("CRC Verifcation Passed\r\n");
	 #endif
		    MCU_ID_NO = (uint16_t)((DBGMCU->IDCODE)&0x00000FFF);

			/* Report chip ID Number */
			HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&MCU_ID_NO, 2, HAL_MAX_DELAY);
	 }
	 else {
	 	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("CRC Verifcation failed\r\n");
	 #endif
		
  BL_Send_NACK();
 
	


	 }
	 
}
	 
static uint8_t Recieved_Address_Verfication(uint32_t JUMP_ADDRESS) {
    uint8_t Addr_Verf = ADDRESS_NOT_VALID;
    if (((JUMP_ADDRESS >= STM32F103_SRAM_BASE) && (JUMP_ADDRESS <= STM32F103_SRAM_END)) ||
        ((JUMP_ADDRESS >= STM32F103_FLASH_BASE) && (JUMP_ADDRESS <= STM32F103_FLASH_END))) {
        Addr_Verf = ADDRESS_VALID;
   
				}
				else {
					Addr_Verf = ADDRESS_NOT_VALID;
		}
    return Addr_Verf;
}



static void handleCBL_GET_RDP_STATUS_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_GET_RDP_STATUS_CMD
    // Add your code here
}

static void handleCBL_GO_TO_ADDR_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_GO_TO_ADDR_CMD
     uint16_t  Host_CMD_Packet_Len   =0;
	   uint32_t  Host_CRC32            =0;
	   uint16_t  MCU_ID_NO             =0;
		 uint32_t  HOST_JUMP_ADDRESS     =0;
		 uint8_t   Addr_Verf             =ADDRESS_NOT_VALID;
	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CBL_GO_TO_ADDR_CMD reached.\r\n");
	 #endif

		 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));
	
/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
		 BL_Send_ACK(1);
		 /*EXTRACT ADDRESS FROM THE HOST PKT*/ 
		 HOST_JUMP_ADDRESS=*((uint32_t *)&BL_HOST_BUFFER[2]);
		 
		 /**Address Verfication**/
		 Addr_Verf=Recieved_Address_Verfication(HOST_JUMP_ADDRESS);
				 if(ADDRESS_VALID==Addr_Verf){
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
				BL_Print_Message("Address Verfication Succedded \r\n");
	 #endif
				HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&Addr_Verf, 1, HAL_MAX_DELAY);
			 Jump_Ptr	Jump_Address = (Jump_Ptr)(HOST_JUMP_ADDRESS+1);
		  Jump_Address();
			 /** it is must that LSB to be 1 **/
		 
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
					 BL_Print_Message("Jump to :0x%X \r\n",(Jump_Address-1));
	 #endif
				 
				 }
		 else {
		 		 	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&Addr_Verf, 1, HAL_MAX_DELAY);
		 
		 }
		  }
	 
			
	 else {
	 	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("CRC Verifcation failed\r\n");
	 #endif
		
		 BL_Send_NACK();
 
	
	 


	 }






}

static uint8_t Perform_Flash_Erase(uint8_t PageAddr,uint8_t Nb_Pages){
		uint8_t Sector_Validity_Status = INVALID_SECTOR_NUMBER;
		uint8_t Remaining_Sectors;
		FLASH_EraseInitTypeDef Init;
		HAL_StatusTypeDef		 HAL_STATUS= HAL_ERROR ;
		uint32_t Sector_Error=0;
	if(Nb_Pages > CBL_MAX_PAGE_NUMBER){
		
		/* ..Sector_Validity_Status = INVALID_SECTOR_NUMBER;..*/
	
	
	}
	else {
		if( (PageAddr <= (CBL_MAX_PAGE_NUMBER -1)) || (CBL_Mass_ERASE == PageAddr)	) {
				if(CBL_Mass_ERASE == PageAddr){
				Init.TypeErase=FLASH_TYPEERASE_MASSERASE;
				}
					else {
						Remaining_Sectors=CBL_MAX_PAGE_NUMBER-PageAddr;
						if(CBL_MAX_PAGE_NUMBER<Nb_Pages) {
							Nb_Pages=Remaining_Sectors;
						}
						else {/*Nothing to be done */}
							Init.TypeErase   = FLASH_TYPEERASE_PAGES;
							Init.PageAddress = PageAddr;
							Init.NbPages     = Nb_Pages;
					}
							Init.Banks=FLASH_BANK_1;
					
					/*UNLOCK THE FLASH CONTROL REGISTER SECTORS */
				HAL_STATUS=HAL_FLASH_Unlock();
					HAL_STATUS	= HAL_FLASHEx_Erase(/*POINTER TO ERASING CONFIGURATION*/ &Init,&Sector_Error);
						if(HAL_SUCCESSFUL_ERASE==Sector_Error){
							Sector_Validity_Status=SUCCESSFUL_ERASE;
						}
						else {Sector_Validity_Status=UNSUCCESSFUL_ERASE;}

							/*LOCK THE FLASH CONTROL REGISTER SECTORS */
							HAL_STATUS=HAL_FLASH_Lock();
		
		}
							
					
			else{
				Sector_Validity_Status=UNSUCCESSFUL_ERASE;
		}
		
							
		
	

	}

return (Sector_Validity_Status);
}



static void handleCBL_FLASH_ERASE_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_FLASH_ERASE_CMD
    uint16_t  Host_CMD_Packet_Len   =0;
	  uint32_t  Host_CRC32            =0;
	  uint32_t  Erase_Status          =0;
	
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CBL_FLASH_ERASE_CMD reached.\r\n");
	 #endif
	
			 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));
	
	
/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
	BL_Send_ACK(1);
		 
		 Erase_Status = Perform_Flash_Erase(BL_HOST_BUFFER[2],BL_HOST_BUFFER[3]);

		 if(SUCCESSFUL_ERASE==Erase_Status) /*Success*/ {
			 	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&Erase_Status, 1, HAL_MAX_DELAY);
		 #if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
					BL_Print_Message("Erasing Done Successfully \r\n");
			#endif
		 }
		 else /*Failure*/ {
			  	HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&Erase_Status, 1, HAL_MAX_DELAY);
		 
		 }
	 
	 
	 }
	 else {
	 	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("CRC Verifcation failed\r\n");
	 #endif
		 BL_Send_NACK();
	 }
		}

		

static uint8_t FLASH_MEM_WRITE_PAYLOAD(uint8_t* HOST_PAYLOAD,uint32_t PAYLOAD_START_ADDR, uint16_t PAYLOAD_LENGTH) {
	
	HAL_StatusTypeDef HAL_STATUS       = HAL_ERROR;
	uint16_t  Payload_Counter    ;
	uint8_t FLASH_PAYLOAD_WRITE_STATUS = FLASH_PAYLOAD_WRITE_FAILED;
	/*UNLOCK FLASH MEMORY*/
	HAL_STATUS=HAL_FLASH_Unlock();
	if(HAL_STATUS != HAL_OK){
				FLASH_PAYLOAD_WRITE_STATUS = FLASH_PAYLOAD_WRITE_FAILED;
			
	}
	else {
	for(Payload_Counter=0;Payload_Counter<PAYLOAD_LENGTH;Payload_Counter++){
	HAL_STATUS=HAL_FLASH_Program(FLASH_TYPEPROGRAM_BYTE, PAYLOAD_START_ADDR+Payload_Counter, HOST_PAYLOAD[Payload_Counter]);
		if(HAL_STATUS != HAL_OK){
				FLASH_PAYLOAD_WRITE_STATUS = FLASH_PAYLOAD_WRITE_FAILED;
			break;
	}
		else{
		FLASH_PAYLOAD_WRITE_STATUS=FLASH_PAYLOAD_WRITE_PASSED;
		}
}
	
  
}
	if(FLASH_PAYLOAD_WRITE_PASSED==FLASH_PAYLOAD_WRITE_STATUS&&HAL_OK==HAL_STATUS)
	{/*LOCK FLASH MEMORY*/
	HAL_STATUS=HAL_FLASH_Lock();
		if(HAL_STATUS != HAL_OK){
				FLASH_PAYLOAD_WRITE_STATUS = FLASH_PAYLOAD_WRITE_FAILED;
			
	}
		else {FLASH_PAYLOAD_WRITE_STATUS=FLASH_PAYLOAD_WRITE_PASSED;}
	}
	return FLASH_PAYLOAD_WRITE_STATUS;
}	
static void handleCBL_MEM_WRITE_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_MEM_WRITE_CMD
    uint16_t  Host_CMD_Packet_Len   =0;
	  uint32_t  Host_CRC32            =0;
		uint32_t  Host_Address          =0;
		 uint8_t   Payload_Len           =0;
		 uint8_t   Addr_Verf            =ADDRESS_NOT_VALID;
		uint8_t FLASH_PAYLOAD_WRITE_STATUS = FLASH_PAYLOAD_WRITE_FAILED;
	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
		BL_Print_Message("CBL_FLASH_ERASE_CMD reached.\r\n");
	 #endif
	
			 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));
	
/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
	BL_Send_ACK(1);
	
		 Host_Address = *((uint32_t*)(&BL_HOST_BUFFER[2])); /*count 4 byte from position 2 in array which is the address */
		 Payload_Len  = BL_HOST_BUFFER[6];
 Addr_Verf= Recieved_Address_Verfication(Host_Address);
		 if(ADDRESS_VALID==Addr_Verf){
		 FLASH_PAYLOAD_WRITE_STATUS= FLASH_MEM_WRITE_PAYLOAD((uint8_t*)&BL_HOST_BUFFER[7],Host_Address,Payload_Len);
		 if(FLASH_PAYLOAD_WRITE_FAILED==FLASH_PAYLOAD_WRITE_STATUS){
		  HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&FLASH_PAYLOAD_WRITE_STATUS, 1, HAL_MAX_DELAY);
		 }
			/*problem*/
		 
			 else/*passed*/{ 
			 HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&FLASH_PAYLOAD_WRITE_STATUS, 1, HAL_MAX_DELAY);}
		 }
		 else/*problem*/{
		 HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&Addr_Verf, 1, HAL_MAX_DELAY);
		 }
	 }
	 
	 else {
	 	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	BL_Print_Message("CRC Verifcation failed\r\n");
	 #endif
		 BL_Send_NACK();
	 }
		}
	
	 static uint8_t CBL_STM32401_Get_RDP_level(uint8_t *RDP_Level) {
	 HAL_StatusTypeDef HAL_SATUS =HAL_ERROR;
	uint8_t RDP_LEVEL_ERROR_STATUS= RDP_LEVEL_READ_INVALID;
		FLASH_OBProgramInitTypeDef FLASH_OBProgram;
		 HAL_FLASHEx_OBGetConfig(&FLASH_OBProgram);
		
	 return FLASH_OBProgram.RDPLevel;
	 }

static void handleCBL_EN_R_W_PROTECT_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for Bootloder_Read_protection_level
  uint16_t  Host_CMD_Packet_Len   =0;
	uint32_t  Host_CRC32            =0;
	uint8_t  RDP_Level             =0;
	#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)   
	BL_Print_Message("CBL_EN_R_W_PROTECT_CMD reached.\r\n");
  #endif
				 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));
	

/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
	BL_Send_ACK(1);
	/* Read Protection Level */	 
RDP_Level=CBL_STM32401_Get_RDP_level(&RDP_Level);
	/*Report Protection Level*/
 HAL_UART_Transmit(BL_HOST_COMMUNICATION_UART, (uint8_t *)&RDP_Level, 1, HAL_MAX_DELAY);
}
	 else {
	 /* Report*/ 
		 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Failed \r\n");
	 #endif
	BL_Send_NACK();
	 }
 }
	 
	 }

static void handleCBL_MEM_READ_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_MEM_READ_CMD
    // Add your code here
}

static void handleCBL_READ_SECTOR_STATUS_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_READ_SECTOR_STATUS_CMD
    // Add your code here
}

static void handleCBL_OTP_READ_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_OTP_READ_CMD
    // Add your code here
}



static void handleCBL_CHANGE_ROP_LEVEL_CMD(uint8_t* BL_HOST_BUFFER) {
    // Implementation for CBL_CHANGE_ROP_LEVEL_CMD
     uint16_t  Host_CMD_Packet_Len   =0;
	   uint32_t  Host_CRC32            =0;
			#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)   
	BL_Print_Message("CBL_EN_R_W_PROTECT_CMD reached.\r\n");
  #endif
				 /*Extract the CRC32 and pkt length sent by Host*/
	 Host_CMD_Packet_Len=BL_HOST_BUFFER[0] + 1;
		Host_CRC32=*((uint32_t*)((BL_HOST_BUFFER+Host_CMD_Packet_Len)-CRC_TYPE_SIZE));
	

/*CRC Verification*/
	 if(CRC_OK == Bootloader_CRC_verify( (uint8_t*)&BL_HOST_BUFFER[0],Host_CMD_Packet_Len-CRC_TYPE_SIZE , Host_CRC32)) {
		#if (BL_DEBUG_ENABLE == DEBUG_INFO_ENABLE)
	 BL_Print_Message("CRC Verifcation Passsed \r\n");
	 #endif
	BL_Send_ACK(1);
}
	 }




BL_status BL_UART_FETCH_HOST_COMMAND(void){
	
	BL_status status=BL_NACK;
	HAL_StatusTypeDef HAL_STATUS=HAL_ERROR;
	uint8_t Data_length =0;
	memset(BL_HOST_BUFFER,0,BL_HOST_BUFFER_length);
	HAL_STATUS=HAL_UART_Receive(BL_HOST_COMMUNICATION_UART,BL_HOST_BUFFER,1,HAL_MAX_DELAY);

	if(HAL_STATUS != HAL_OK) {
			status =BL_NACK;
	
	}
	else {
				Data_length=BL_HOST_BUFFER[0];
			HAL_STATUS=HAL_UART_Receive(BL_HOST_COMMUNICATION_UART,&BL_HOST_BUFFER[1],Data_length,HAL_MAX_DELAY);
			if(HAL_STATUS != HAL_OK) {
			status =BL_NACK;
	}
		else {
switch (BL_HOST_BUFFER[1]) {
    case CBL_GET_VER_CMD:
        // Code to handle CBL_GET_VER_CMD
        BL_Print_Message("CBL_GET_VER_CMD reached.\r\n");
        handleCBL_GET_VER_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_GET_HELP_CMD:
        // Code to handle CBL_GET_HELP_CMD
        
        handleCBL_GET_HELP_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_GET_CID_CMD:
        // Code to handle CBL_GET_CID_CMD
        
        handleCBL_GET_CID_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_GET_RDP_STATUS_CMD:
        // Code to handle CBL_GET_RDP_STATUS_CMD
        BL_Print_Message("CBL_GET_RDP_STATUS_CMD reached.\r\n");
        handleCBL_GET_RDP_STATUS_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_GO_TO_ADDR_CMD:
        // Code to handle CBL_GO_TO_ADDR_CMD
     
        handleCBL_GO_TO_ADDR_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_FLASH_ERASE_CMD:
        // Code to handle CBL_FLASH_ERASE_CMD
        
        handleCBL_FLASH_ERASE_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_MEM_WRITE_CMD:
        // Code to handle CBL_MEM_WRITE_CMD

        handleCBL_MEM_WRITE_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_EN_R_W_PROTECT_CMD:
        // Code to handle CBL_EN_R_W_PROTECT_CMD
       
        handleCBL_EN_R_W_PROTECT_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_MEM_READ_CMD:
        // Code to handle CBL_MEM_READ_CMD
        BL_Print_Message("CBL_MEM_READ_CMD reached.\r\n");
        handleCBL_MEM_READ_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_READ_SECTOR_STATUS_CMD:
        // Code to handle CBL_READ_SECTOR_STATUS_CMD
        BL_Print_Message("CBL_READ_SECTOR_STATUS_CMD reached.\r\n");
        handleCBL_READ_SECTOR_STATUS_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_OTP_READ_CMD:
        // Code to handle CBL_OTP_READ_CMD
        BL_Print_Message("CBL_OTP_READ_CMD reached.\r\n");
        handleCBL_OTP_READ_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    case CBL_CHANGE_ROP_LEVEL_CMD:
        // Code to handle CBL_CHANGE_ROP_LEVEL_CMD
        BL_Print_Message("CBL_CHANGE_ROP_LEVEL_CMD reached.\r\n");
        handleCBL_CHANGE_ROP_LEVEL_CMD(BL_HOST_BUFFER);
        status = BL_OK;
        break;
    default:
        // Code to handle unknown command
        BL_Print_Message("Unknown command reached !!\r\n");
        status = BL_OK;
        break;
}




		
		}	
	
}
	return status;

};

static void bootloader_Jump_to_User_App(void){
			
	/* Value of the main stack pointer of main application */
	uint32_t MSP_Value=*((volatile uint32_t *)(FLASH_SECTOR2_BASE_ADDRESS));
	
	/* Reset Handler defination functionof main application */
	uint32_t MainAppAddr=*((volatile uint32_t *)(FLASH_SECTOR2_BASE_ADDRESS+4));

	/*Fetch the reset Handler address of user application*/
	/**x**/ /* void(*pMainApp)(void)=(void*)MainAppAddr;*/
	pMainApp ResetHandler_Address =(pMainApp) MainAppAddr;

	/** Set Main Stack Pointer **/ 
	__set_MSP(MSP_Value);
	
	/** Deintia;ize of modules **/
	HAL_RCC_DeInit();  /* Resets the RCC clock configuration to the default reset state */
 /** Jump to Application Reset Handler **/
  ResetHandler_Address();
}



void BL_Print_Message(char *format, ...){
	char Message[100] ={0};
	va_list args;
	va_start(args,format);
	vsprintf(Message, format, args);
	#if (DEBUG_METHOD_UART==DEBUG_METHOD)
	HAL_UART_Transmit(&huart2, (uint8_t*)Message, sizeof(Message),HAL_MAX_DELAY);
	#elif (DEBUG_METHOD_SPI==DEBUG_METHOD)
	/**PERFORMS BL DEBUGGING USING SPI**/
	#elif (DEBUG_METHOD_CAN==DEBUG_METHOD)
	/**PERFORMS BL DEBUGGING USING CAN**/
	#endif
}

/*------------------ Static Functions Definitions END -----------------*/




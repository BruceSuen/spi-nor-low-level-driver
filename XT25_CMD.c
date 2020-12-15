/* XT25_CMD.c - SPI and QPI interface command set */
 
/**************************************************************************
* Copyright 2020 XTX Technology Co., Ltd. All Rights Reserved. 
*
* This software is owned and published by XTX Technology Co., Ltd
*
* BY DOWNLOADING, INSTALLING OR USING THIS SOFTWARE, YOU AGREE TO BE BOUND 
* BY ALL THE TERMS AND CONDITIONS OF THIS AGREEMENT.
*
* This software constitutes driver source code for use in programming XTX's 
* Flash memory components. This software is licensed by XTX to be adapted only 
* for use in systems utilizing XTX's Flash memories. XTX is not be 
* responsible for misuse or illegal use of this software for devices not 
* supported herein.  XTX is providing this source code "AS IS" and will 
* not be responsible for issues arising from incorrect user implementation 
* of the source code herein.  
*
* XTX MAKES NO WARRANTY, EXPRESS OR IMPLIED, ARISING BY LAW OR OTHERWISE, 
* REGARDING THE SOFTWARE, ITS PERFORMANCE OR SUITABILITY FOR YOUR INTENDED 
* USE, INCLUDING, WITHOUT LIMITATION, NO IMPLIED WARRANTY OF MERCHANTABILITY, 
* FITNESS FOR A  PARTICULAR PURPOSE OR USE, OR NONINFRINGEMENT.  XTX WILL 
* HAVE NO LIABILITY (WHETHER IN CONTRACT, WARRANTY, TORT, NEGLIGENCE OR 
* OTHERWISE) FOR ANY DAMAGES ARISING FROM USE OR INABILITY TO USE THE SOFTWARE, 
* INCLUDING, WITHOUT LIMITATION, ANY DIRECT, INDIRECT, INCIDENTAL, 
* SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS OF DATA, SAVINGS OR PROFITS, 
* EVEN IF XTX HAS BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.  
*
* This software may be replicated in part or whole for the licensed use, 
* with the restriction that this Copyright notice must be included with 
* this software, whether used in part or whole, at all times.  
*/

#include "XT25_CMD.h"
#include "XT25_HAL_EXAMPLE.h"


FlashStatus gFlashStatus;


/* Utility functions */

/*
 * Function:       Wait_Flash_Ready
 * Arguments:      ExpectTime, expected time-out value of flash operations.
 *           
 *                 fsptr, pointer of flash status structure
 *                 No use at non-synchronous IO mode.
 * Description:    Synchronous IO:
 *                 If flash is ready return TRUE.
 *                 If flash is time-out return FALSE.
 *                
 * Return Value: TRUE, FALSE
 */
BOOL Wait_Flash_Ready( uint32  ExpectTime, FlashStatus *fsptr )
{

    uint32 temp = 0;
    while( IsFlashBusy( fsptr ) )
    {
        if( temp > ExpectTime )
        {
            return FALSE;
        }
        temp = temp + 1;
    }
    return TRUE;

}

/*
 * Function:       IsFlashBusy
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    Check status register WIP bit.
 *                 If  WIP bit = 1: return TRUE ( Busy )
 *                             = 0: return FALSE ( Ready ).
 * Return Value: TRUE, FALSE
 */
BOOL IsFlashBusy( FlashStatus *fsptr )
{
    uint8 gDataBuffer;

    SPI_NOR_RDSR( &gDataBuffer, fsptr );
    if( (gDataBuffer & FLASH_WIP_MASK)  == FLASH_WIP_MASK )
        return TRUE;
    else
        return FALSE;
}
/*
 * Function:       IsFlashQPI
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    Return flash current state: SPI or QPI
 * Return Value: QPI enable or not ( TRUE / FALSE )
 */
BOOL IsFlashQPI( FlashStatus *fsptr )
{
    if( (fsptr->ModeReg & 0x01) == 0x01 )
        return TRUE;
    else
        return FALSE;
}

/*
 * Function:       IsFlashQIO
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    If flash QE bit = 1: return TRUE
 *                                 = 0: return FALSE.
 * Return Value: TRUE, FALSE
 */
BOOL IsFlashQIO( FlashStatus *fsptr )
{
    uint8 gDataBuffer;
    SPI_NOR_RDSR2( &gDataBuffer, fsptr );
    if( (gDataBuffer & FLASH_QE_MASK) == FLASH_QE_MASK )
        return TRUE;
    else
        return FALSE;
}

/*
 * Function:       IsFlash4Byte
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    Check flash address is 3-byte or 4-byte.
 *                 If flash 4BYTE bit = 1: return TRUE
 *                                    = 0: return FALSE.
 * Return Value: TRUE, FALSE
 */
BOOL IsFlash4Byte( FlashStatus *fsptr )
{
	#ifdef SUPPORT_4BYTE_MODE
	uint8 gDataBuffer;
	SPI_NOR_RDSR2( &gDataBuffer, fsptr );
	if( (gDataBuffer & FLASH_4BYTE_MASK) == FLASH_4BYTE_MASK )
		return TRUE;
	else
		return FALSE;
	#else
		
	return FALSE;
	#endif
	
}

/*
 * Function:       SPI_NOR_QUAD_ENABLE
 * Arguments:      fsptr, pointer of flash status structure
 * Description:     set QE bit
 *                
 * Return Value: NONE
 */
void SPI_NOR_QUAD_ENABLE(FlashStatus * fsptr)
{
    uint8 reg[2];
	#ifdef SUPPORT_WRSR_2BYTES
    SPI_NOR_RDSR(&reg[0],fsptr);
	SPI_NOR_RDSR2(&reg[1],fsptr);
    SPI_NOR_WRSR( reg[0]|((reg[1]|FLASH_QE_MASK)<<8), fsptr );

	#else
    SPI_NOR_RDSR2(&reg[0],fsptr);
	SPI_NOR_WRSR2(reg[0]|FLASH_QE_MASK, fsptr);
	#endif
}

/*
 * Function:       SPI_NOR_QUAD_DISABLE
 * Arguments:      fsptr, pointer of flash status structure
 * Description:     Clear QE bit
 *                
 * Return Value: NONE
 */
void SPI_NOR_QUAD_DISABLE(FlashStatus * fsptr)
{
    uint8 reg[2];
	#ifdef SUPPORT_WRSR_2BYTES

	SPI_NOR_RDSR(&reg[0],fsptr);
	SPI_NOR_RDSR2(&reg[1],fsptr);
    SPI_NOR_WRSR( reg[0]|((reg[1]&(~FLASH_QE_MASK))<<8), fsptr);

	#else
    SPI_NOR_RDSR2(&reg[0],fsptr);
	SPI_NOR_WRSR2(reg[0]&(~FLASH_QE_MASK), fsptr);

	#endif
}

/*
 * Function:       SPI_NOR_WPS_ENABLE
 * Arguments:      fsptr, pointer of flash status structure
 * Description:     Set WPS bit
 *                
 * Return Value: NONE
 */
void SPI_NOR_WPS_ENABLE(FlashStatus * fsptr)
{
    uint8 reg[2];
	#ifdef SUPPORT_WRSR_2BYTES
    SPI_NOR_RDSR(&reg[0],fsptr);
	SPI_NOR_RDSR2(&reg[1],fsptr);
    SPI_NOR_WRSR( reg[0]|((reg[1]|FLASH_WPS_MASK)<<8), fsptr );

	#else
    SPI_NOR_RDSR2(&reg[0],fsptr);
	SPI_NOR_WRSR2(reg[0]|FLASH_WPS_MASK, fsptr);
	#endif
}


/*
 * Function:       SPI_NOR_WPS_DISABLE
 * Arguments:      fsptr, pointer of flash status structure
 * Description:     Clear WPS bit
 *                
 * Return Value: NONE
 */
void SPI_NOR_WPS_DISABLE(FlashStatus * fsptr)
{
    uint8 reg[2];
	#ifdef SUPPORT_WRSR_2BYTES

	SPI_NOR_RDSR(&reg[0],fsptr);
	SPI_NOR_RDSR2(&reg[1],fsptr);
    SPI_NOR_WRSR( reg[0]|((reg[1]&(~FLASH_WPS_MASK))<<8), fsptr);

	#else
    SPI_NOR_RDSR2(&reg[0],fsptr);
	SPI_NOR_WRSR2(reg[0]&(~FLASH_WPS_MASK), fsptr);

	#endif
}

/*
 * Function:       SPI_NOR_LOCK_BIT_ENABLE
 * Arguments:      fsptr, pointer of flash status structure
 * Description:     Set Lock bit
 *                
 * Return Value: NONE
 */

void SPI_NOR_LOCK_BIT_ENABLE(uint8 BitMask,FlashStatus * fsptr)
{

	uint8 reg[2];
	#ifdef SUPPORT_WRSR_2BYTES
    SPI_NOR_RDSR(&reg[0],fsptr);
	SPI_NOR_RDSR2(&reg[1],fsptr);
    SPI_NOR_WRSR( reg[0]|((reg[1]|BitMask)<<8), fsptr );

	#else
    SPI_NOR_RDSR2(&reg[0],fsptr);
	SPI_NOR_WRSR2(reg[0]|BitMask, fsptr);
	#endif

}




/*
 * ID Command
 */

/*
 * Function:       SPI_NOR_RDID
 * Arguments:      Identification, 32 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The RDID instruction is to read the manufacturer ID
 *                 of 1-byte and followed by Device ID of 2-byte.
 * Return Value: LLD_CmdSpiOnly, LLD_OK
 */
LLD_STATUS SPI_NOR_RDID( uint32 *Identification, FlashStatus *fsptr )
{
	  uint8 gDataBuffer[3];

    // Check current state: SPI or QPI
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

		FLASH_READ(SPI_NOR_CMD_RDID,ADDRESS_NULL,0,IO_NULL,SIO,gDataBuffer,sizeof(gDataBuffer),fsptr);
		*Identification = gDataBuffer[0] << 16 | gDataBuffer[1] << 8 | gDataBuffer[2];

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_RES
 * Arguments:      ElectricIdentification, 8 bit buffer to store electric id
 *                 fsptr, pointer of flash status structure
 * Description:    The RES instruction is to read the Device
 *                 electric identification of 1-byte.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RES( uint8 *ElectricIdentification, FlashStatus *fsptr )
{
	FLASH_READ(SPI_NOR_CMD_RES,ADDRESS_NULL,24,IO_NULL,SIO,ElectricIdentification,1,fsptr);
   return LLD_OK;
}

/*
 * Function:       SPI_NOR_REMS
 * Arguments:      REMS_Identification, 16 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The REMS instruction is to read the Device
 *                 manufacturer ID and electric ID of 1-byte.
 * Return Value: LLD_CmdSpiOnly, LLD_OK
 */
LLD_STATUS SPI_NOR_REMS( uint16 *REMS_Identification, FlashStatus *fsptr )
{
    uint8 gDataBuffer[2];

	  FLASH_READ(SPI_NOR_CMD_REMS,0x00,0,SIO,SIO,gDataBuffer,sizeof(gDataBuffer),fsptr);
    // Store identification information
    *REMS_Identification = (gDataBuffer[0] << 8) | gDataBuffer[1];
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_REMS_DIO
 * Arguments:      REMS_Identification, 16 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The REMS instruction is to read the Device
 *                 manufacturer ID and electric ID of 1-byte DIO mode.
 * Return Value: LLD_CmdSpiOnly, LLD_OK
 */
LLD_STATUS SPI_NOR_REMS_DIO( uint16 *REMS_Identification, FlashStatus *fsptr )
{
    uint8 gDataBuffer[2];

    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;
	
	 FLASH_READ(SPI_NOR_CMD_REMSDIO,0,4,DIO,DIO,gDataBuffer,sizeof(gDataBuffer),fsptr);
	
    // Store identification information
    *REMS_Identification = (gDataBuffer[0] << 8) | gDataBuffer[1];
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_REMS_QIO
 * Arguments:      REMS_Identification, 16 bit buffer to store id
 *                 fsptr, pointer of flash status structure
 * Description:    The REMS instruction is to read the Device
 *                 manufacturer ID and electric ID of 1-byte QIO mode.
 * Return Value: LLD_CmdSpiOnly, LLD_OK
 */
LLD_STATUS SPI_NOR_REMS_QIO( uint16 *REMS_Identification, FlashStatus *fsptr )
{
    uint8 gDataBuffer[2];

    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;
	
	 FLASH_READ(SPI_NOR_CMD_REMSQIO,0,6,QIO,QIO,gDataBuffer,sizeof(gDataBuffer),fsptr);
	
    // Store identification information
    *REMS_Identification = (gDataBuffer[0] << 8) | gDataBuffer[1];
    return LLD_OK;
}


/*
 * Register Command
 */
/*
 * Function:       SPI_NOR_RDSR
 * Arguments:      StatusReg, 8 bit buffer to store status register value
 *                 fsptr, pointer of flash status structure
 * Description:    The RDSR instruction is for reading Status
 *                 Register Bits.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RDSR( uint8 *StatusReg, FlashStatus *fsptr )
{
	 FLASH_READ(SPI_NOR_CMD_RDSR,ADDRESS_NULL,0,IO_NULL,SIO,StatusReg,1,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_RDSR2
 * Arguments:      StatusReg, 8 bit buffer to store status register value
 *                 fsptr, pointer of flash status structure
 * Description:    The RDSR instruction is for reading Status
 *                 Register Bits.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RDSR2( uint8 *StatusReg, FlashStatus *fsptr )
{

	FLASH_READ(SPI_NOR_CMD_RDSR2,ADDRESS_NULL,0,IO_NULL,SIO,StatusReg,1,fsptr);
   return LLD_OK;
}

/*
 * Function:       SPI_NOR_RDSR3
 * Arguments:      StatusReg, 8 bit buffer to store status register value
 *                 fsptr, pointer of flash status structure
 * Description:    The RDSR instruction is for reading Status
 *                 Register Bits.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RDSR3( uint8 *StatusReg, FlashStatus *fsptr )
{
	FLASH_READ(SPI_NOR_CMD_RDSR3,ADDRESS_NULL,0,IO_NULL,SIO,StatusReg,1,fsptr);
  return LLD_OK;
}

/*
 * Function:       SPI_NOR_WRSR
 * Arguments:      UpdateValue, 8/16 bit status register value to update
 *                 fsptr, pointer of flash status structure
 * Description:    The WRSR instruction is for changing the values of
 *                 Status Register Bits
 * Return Value: LLD_FlashBusy, LLD_TimeOut, LLD_OK
 */
#ifdef SUPPORT_WRSR_2BYTES
LLD_STATUS SPI_NOR_WRSR( uint16 UpdateValue, FlashStatus *fsptr )
#else
LLD_STATUS SPI_NOR_WRSR( uint8 UpdateValue, FlashStatus *fsptr )
#endif

{
	uint8 gDataBuffer[2];
    // Check flash is busy or not
	gDataBuffer[0] = (uint8)UpdateValue;

    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );
	#ifdef SUPPORT_WRSR_2BYTES
	
		gDataBuffer[1] = (uint8)(UpdateValue>>8);
		FLASH_WRITE(SPI_NOR_CMD_WRSR,ADDRESS_NULL,IO_NULL,SIO,gDataBuffer,2,fsptr);
	
	#else
    	FLASH_WRITE(SPI_NOR_CMD_WRSR,ADDRESS_NULL,IO_NULL,SIO,gDataBuffer,1,fsptr);
  #endif

    if( Wait_Flash_Ready( WriteStatusRegCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_WRSR2
 * Arguments:      UpdateValue, 8 bit status register value to update
 *                 fsptr, pointer of flash status structure
 * Description:    The WRSR instruction is for changing the values of
 *                 Status Register Bits
 * Return Value: LLD_FlashBusy, LLD_TimeOut, LLD_OK
 */

LLD_STATUS SPI_NOR_WRSR2( uint8 UpdateValue, FlashStatus *fsptr )
{

    // Check flash is busy or not

    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

	FLASH_WRITE(SPI_NOR_CMD_WRSR2,ADDRESS_NULL,IO_NULL,SIO,&UpdateValue,1,fsptr);
	
    if( Wait_Flash_Ready( WriteStatusRegCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_WRSR3
 * Arguments:      UpdateValue, 8 bit status register value to update
 *                 fsptr, pointer of flash status structure
 * Description:    The WRSR instruction is for changing the values of
 *                 Status Register Bits
 * Return Value: LLD_FlashBusy, LLD_TimeOut, LLD_OK
 */

LLD_STATUS SPI_NOR_WRSR3( uint8 UpdateValue, FlashStatus *fsptr )
{

    // Check flash is busy or not

    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

	FLASH_WRITE(SPI_NOR_CMD_WRSR3,ADDRESS_NULL,IO_NULL,SIO,&UpdateValue,1,fsptr);
	
    if( Wait_Flash_Ready( WriteStatusRegCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}


/*
 * Function:       SPI_NOR_RDEAR
 * Arguments:      ExtReg, 8 bit buffer to store extend address register value
 *                 fsptr, pointer of flash status structure
 * Description:    The RDEAR instruction is for reading Extended Address 
 *                 Register Bits.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RDEAR( uint8 *ExtReg, FlashStatus *fsptr )
{
	FLASH_READ(SPI_NOR_CMD_RDEAR,ADDRESS_NULL,0,IO_NULL,SIO,ExtReg,1,fsptr);
  return LLD_OK;
}

/*
 * Function:       SPI_NOR_WREAR
 * Arguments:      UpdateValue, 8 bit data of new extend address register value
 *                 fsptr, pointer of flash status structure
 * Description:    The WREAR instruction is for writing Extended Address 
 *                 Register Bits.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_WREAR( uint8 UpdateValue, FlashStatus *fsptr )
{

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_WREAR,ADDRESS_NULL,IO_NULL,SIO,&UpdateValue,1,fsptr);

    if( Wait_Flash_Ready( WriteExtRegCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}




/*
 * Read Command
 */
/*
 * Function:       SPI_NOR_READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The READ instruction is for reading data out.
 * Return Value: LLD_CmdSpiOnly, LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{

    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE ) return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_READ(SPI_NOR_CMD_READ,flash_address,0,SIO,SIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_READ4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The READ4B instruction is for reading data out with 4 byte address.
 * Return Value: LLD_CmdSpiOnly, LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_READ4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE ) return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    FLASH_READ(SPI_NOR_CMD_READ4B,flash_address,0,SIO,SIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_2READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 2READ instruction enable double throughput of Serial
 *                 Flash in read mode
 * Return Value: LLD_CmdSpiOnly, LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_2READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
	uint8 dc = 4;
    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE )    return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );
    FLASH_READ(SPI_NOR_CMD_2READ,flash_address,dc,DIO,DIO,target_address,byte_length,fsptr);
    return LLD_OK;
}


/*
 * Function:       SPI_NOR_2READ4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 2READ4B instruction enable double throughput of Serial
 *                 Flash in read mode with 4 byte address.
 * Return Value: LLD_CmdSpiOnly, LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_2READ4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 4;
    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE )    return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;
    FLASH_READ(SPI_NOR_CMD_READ4B,flash_address,dc,DIO,DIO,target_address,byte_length,fsptr);
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_4READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 4READ instruction enable quad throughput of
 *                 Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_QuadNotEnable, LLD_OK
 */
LLD_STATUS SPI_NOR_4READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
	uint8 dc;
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;
	
	 if( IsFlashQPI( fsptr ) ){
		 dc = 8;
	 }
	 else{
		 dc = 6;
	 }

     // Check QE bit
    if( IsFlashQIO( fsptr ) != TRUE )  return LLD_QuadNotEnable;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );
    FLASH_READ(SPI_NOR_CMD_4READ,flash_address,dc,QIO,QIO,target_address,byte_length,fsptr);
    
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_4READ4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 4READ4B instruction enable quad throughput of
 *                 Serial Flash in read mode with 4 byte address.
 * Return Value: LLD_AddressInvalid, LLD_QuadNotEnable, LLD_OK
 */
LLD_STATUS SPI_NOR_4READ4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc;
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;
	
	 if( IsFlashQPI( fsptr ) ){
		 dc = 8;
	 }
	 else{
		 dc = 6;
	 }

     // Check QE bit
    if( IsFlashQIO( fsptr ) != TRUE )  return LLD_QuadNotEnable;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;
    FLASH_READ(SPI_NOR_CMD_4READ4B,flash_address,dc,QIO,QIO,target_address,byte_length,fsptr);
    
    return LLD_OK;
}


/*
 * Function:       SPI_NOR_DREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The DREAD instruction enable double throughput of Serial
 *                 Flash in read mode
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_DREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 8;

    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE )    return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_READ(SPI_NOR_CMD_DREAD,flash_address,dc,SIO,DIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_DREAD4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The DREAD4B instruction enable double throughput of Serial
 *                 Flash in read mode with 4 byte addr
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_DREAD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 8;

    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE )    return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;
	
	fsptr->addr_4byte_mode = TRUE;
    FLASH_READ(SPI_NOR_CMD_DREAD4B,flash_address,dc,SIO,DIO,target_address,byte_length,fsptr);
     
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_QREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The QREAD instruction enable quad throughput of
 *                 Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_QuadNotEnable, LLD_OK
 */
LLD_STATUS SPI_NOR_QREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 8;

    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE )    return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check QE bit
    if( IsFlashQIO( fsptr ) != TRUE )  return LLD_QuadNotEnable;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );
	FLASH_READ(SPI_NOR_CMD_QREAD,flash_address,dc,SIO,QIO,target_address,byte_length,fsptr);

   
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_QREAD4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The QREAD4B instruction enable quad throughput of
 *                 Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_QuadNotEnable, LLD_OK
 */
LLD_STATUS SPI_NOR_QREAD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 8;

    // Check current state
    if( IsFlashQPI( fsptr ) != FALSE )    return LLD_CmdSpiOnly;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check QE bit
    if( IsFlashQIO( fsptr ) != TRUE )  return LLD_QuadNotEnable;
	
	fsptr->addr_4byte_mode = TRUE;
    FLASH_READ(SPI_NOR_CMD_QREAD4B,flash_address,dc,SIO,QIO,target_address,byte_length,fsptr);
    
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_FASTREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The FASTREAD instruction is for quickly reading data out.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_FASTREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc;
    
	if( IsFlashQPI( fsptr ) ){
		dc = 8;
	}
	else{
		dc = 8;
	}
	
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_READ(SPI_NOR_CMD_FASTREAD,flash_address,dc,SIO,SIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_FASTREAD4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The FASTREAD4B instruction is for quickly reading data out with 4 byte address.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_FASTREAD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
   uint8 dc;
    
	if( IsFlashQPI( fsptr ) ){
		dc = 8;
	}
	else{
		dc = 8;
	}
	
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    FLASH_READ(SPI_NOR_CMD_FASTREAD4B,flash_address,dc,SIO,SIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_1DTRD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 1DTRD instruction enables Double Transfer Rate
 *                 throughput on qual I/O of Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_1DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
  
    uint8 dc;
	
	if( IsFlashQPI( fsptr ) ){
		dc = 8;
	}
	else{
		dc = 6;
	}
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
   fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

   FLASH_READ(SPI_NOR_CMD_1DTRD,flash_address,dc,DTSIO,DTSIO,target_address,byte_length,fsptr);

    return LLD_OK;
}



/*
 * Function:       SPI_NOR_DTRD_WRAPQPI
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The DTRD_WRAPQPI instruction enables DT wrap read in qpi mode
 *                 throughput on qual I/O of Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_DTRD_WRAPQPI( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
  
    uint8 dc = 8;
	
	if( !IsFlashQPI( fsptr ) ){
		return LLD_CmdQpiOnly;
	}

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = FALSE;

   FLASH_READ(SPI_NOR_CMD_DTRD_WRAPQPI,flash_address,dc,DTQIO,DTQIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_2DTRD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 2DTRD instruction enables Double Transfer Rate
 *                 throughput on qual I/O of Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_2DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
  
    uint8 dc = 6;
	
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
   fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

   FLASH_READ(SPI_NOR_CMD_2DTRD,flash_address,dc,DTDIO,DTDIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_4DTRD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 4DTRD instruction enables Double Transfer Rate
 *                 throughput on qual I/O of Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_QuadNotEnable, LLD_OK
 */
LLD_STATUS SPI_NOR_4DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
  
    uint8 dc = 8;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
   fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

   FLASH_READ(SPI_NOR_CMD_4DTRD,flash_address,dc,DTQIO,DTQIO,target_address,byte_length,fsptr);

    return LLD_OK;
}


/*
 * Function:       SPI_NOR_4DTRD4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The 4DTRD4B instruction enables Double Transfer Rate
 *                 throughput on qual I/O of Serial Flash in read mode.
 * Return Value: LLD_AddressInvalid, LLD_QuadNotEnable, LLD_OK
 */
LLD_STATUS SPI_NOR_4DTRD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 8;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    fsptr->addr_4byte_mode = TRUE;
    FLASH_READ(SPI_NOR_CMD_4DTRD4B,flash_address,dc,DTQIO,DTQIO,target_address,byte_length,fsptr);
    return LLD_OK;
}


/*
 * Function:       SPI_NOR_WRAPRD_QPI
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The WRAPQPI instruction is for wrap read in qpi mode.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_WRAPRD_QPI( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 4;
    
	if( !IsFlashQPI( fsptr ) ){

		return LLD_CmdQpiOnly;
	}

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = FALSE;

    FLASH_READ(SPI_NOR_CMD_WRAPQPI,flash_address,dc,QIO,QIO,target_address,byte_length,fsptr);

    return LLD_OK;
}



/*
 * Function:       SPI_NOR_RDSFDP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    RDSFDP can retrieve the operating characteristics, structure
 *                 and vendor-specified information such as identifying information,
 *                 memory size, operating voltages and timinginformation of device
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_RDSFDP( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc;
	
	if( IsFlashQPI( fsptr ) ){
		dc = 6;
	}
	else{
		dc = 8;
	}

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    fsptr->addr_4byte_mode = FALSE;
    FLASH_READ(SPI_NOR_CMD_RDSFDP,flash_address,dc,SIO,SIO,target_address,byte_length,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_RDUID
 * Arguments:   
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    RDUID can read the uinique id of flash
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_RDUID(uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 32;

    fsptr->addr_4byte_mode = FALSE;
    FLASH_READ(SPI_NOR_CMD_RDUID,ADDRESS_NULL,dc,IO_NULL,SIO,target_address,byte_length,fsptr);
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_READ_SECR
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 *                 fsptr, pointer of flash status structure
 * Description:    The READ_SECR instruction is for  reading Security Register.
 * Return Value: LLD_AddressInvalid, LLD_OK
 */
LLD_STATUS SPI_NOR_READ_SECR( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr )
{
    uint8 dc = 8;
 
    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_READ(SPI_NOR_CMD_RDSECR,flash_address,dc,SIO,SIO,target_address,byte_length,fsptr);

    return LLD_OK;
}
/*
 * Program Command
 */

/*
 * Function:       SPI_NOR_WREN
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The WREN instruction is for setting rite Enable Latch
 *                 (WEL) bit.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_WREN( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_WREN,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_WREN_VOL
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The WREN instruction is for setting rite Enable Latch
 *                 (WEL) bit.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_WREN_VOL( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_WREN_VOL,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_WRDI
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The WRDI instruction is to reset
 *                 Write Enable Latch (WEL) bit.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_WRDI( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_WRDI,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}


/*
 * Function:       SPI_NOR_PP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The PP instruction is for programming
 *                 the memory to be "0".
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_PP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;


    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_PP,flash_address,SIO,SIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_PP4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The PP4B instruction is for programming
 *                 the memory to be "0" with 4 byte address.
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_PP4B( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;


    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_PP4B,flash_address,SIO,SIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_4PP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The 4PP instruction is for programming the memory to be "0".
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_CmdSpiOnly, LLD_QuadNotEnable,LLD_AddressInvalid
 *                 LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_4PP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{

    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

    // Check QE bit
    if( !IsFlashQIO( fsptr ) ) return LLD_QuadNotEnable;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

   FLASH_WRITE(SPI_NOR_CMD_4PP,flash_address,QIO,QIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}




/*
 * Function:       SPI_NOR_4PP4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The 4PP4B instruction is for programming the memory to be "0" with 4 byte address.
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_CmdSpiOnly, LLD_QuadNotEnable,LLD_AddressInvalid
 *                 LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_4PP4B( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{

    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

    // Check QE bit
    if( !IsFlashQIO( fsptr ) ) return LLD_QuadNotEnable;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

   FLASH_WRITE(SPI_NOR_CMD_4PP,flash_address,QIO,QIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_4PP_32H
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The 4PP instruction is for programming the memory to be "0".
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_CmdSpiOnly, LLD_QuadNotEnable,LLD_AddressInvalid
 *                 LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_4PP_32H( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{

    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

    // Check QE bit
    if( !IsFlashQIO( fsptr ) ) return LLD_QuadNotEnable;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

   FLASH_WRITE(SPI_NOR_CMD_4PP_32H,flash_address,SIO,QIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}




/*
 * Function:       SPI_NOR_4PP_32H4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The 4PP4B instruction is for programming the memory to be "0" with 4 byte address.
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_CmdSpiOnly, LLD_QuadNotEnable,LLD_AddressInvalid
 *                 LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_4PP_32H4B( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{

    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

    // Check QE bit
    if( !IsFlashQIO( fsptr ) ) return LLD_QuadNotEnable;

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

   FLASH_WRITE(SPI_NOR_CMD_4PP_32H4B,flash_address,SIO,QIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}


/*
 * Function:       SPI_NOR_PPSECR
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to programm
 *                 byte_length, byte length of data to programm
 *                 fsptr, pointer of flash status structure
 * Description:    The PPSECR instruction is for programming Security Register.
 *                 the memory to be "0".
 *                 The device only accept the last 256 byte ( one page ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_PPSECR( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr )
{
    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;


    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_PPSECR,flash_address,SIO,SIO,source_address,byte_length,fsptr);

    if( Wait_Flash_Ready( PageProgramCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}


/*
 * Erase Command
 */

/*
 * Function:       SPI_NOR_SE
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The SE instruction is for erasing the data
 *                 of the chosen sector (4KB) to be "1".
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_SE( uint32 flash_address, FlashStatus *fsptr )
{
    
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;


    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_SE,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( SectorEraseCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_SE4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The SE4B instruction is for erasing the data
 *                 of the chosen sector (4KB) to be "1".
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_SE4B( uint32 flash_address, FlashStatus *fsptr )
{
  // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;


    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_SE4B,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( SectorEraseCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_BE32K
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The BE32K instruction is for erasing the data
 *                 of the chosen sector (32KB) to be "1".
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_BE32K( uint32 flash_address, FlashStatus *fsptr )
{
   
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_BE32K,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( BlockErase32KCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_BE32K4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The BE32K4B instruction is for erasing the data
 *                 of the chosen sector (32KB) to be "1".
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_BE32K4B( uint32 flash_address, FlashStatus *fsptr )
{

    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

     // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = TRUE;

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_BE32K4B,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( BlockErase32KCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_BE
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The BE instruction is for erasing the data
 *                 of the chosen sector (64KB) to be "1".
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_BE( uint32 flash_address, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

     // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_BE,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( BlockEraseCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_BE4B
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The BE4B instruction is for erasing the data
 *                 of the chosen sector (64KB) to be "1" with 4 byte address.
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_BE4B( uint32 flash_address, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

     // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_BE4B,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( BlockEraseCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}

/*
 * Function:       SPI_NOR_CE
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The CE instruction is for erasing the data
 *                 of the whole chip to be "1".
 * Return Value: LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_CE( FlashStatus *fsptr )
{
 
    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

   
    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

   FLASH_WRITE(SPI_NOR_CMD_CE,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( ChipEraseCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}


/*
 * Function:       SPI_NOR_ESECR
 * Arguments:      flash_address, 32 bit flash memory address
 *                 fsptr, pointer of flash status structure
 * Description:    The ESECR instruction is for erasing  Security Register.
 *              
 * Return Value: LLD_AddressInvalid, LLD_FlashBusy, LLD_OK,
 *                 LLD_TimeOut
 */
LLD_STATUS SPI_NOR_ESECR( uint32 flash_address, FlashStatus *fsptr )
{
 

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;


    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    // Setting Write Enable Latch bit
    SPI_NOR_WREN( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_ESECR,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    if( Wait_Flash_Ready( SectorEraseCycleTime, fsptr ) )
        return LLD_OK;
    else
        return LLD_TimeOut;
}


/*
 * Mode setting Command
 */



/*
 * Function:       SPI_NOR_DP
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The DP instruction is for setting the
 *                 device on the minimizing the power consumption.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_DP( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_DP,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_RDP
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The RDP instruction is putting the device in the
 *                 Stand-by Power mode.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RDP( FlashStatus *fsptr )
{
	FLASH_WRITE(SPI_NOR_CMD_RDP,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}



/*
 * Function:       SPI_NOR_EQIO
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    Enable QPI mode
 * Return Value: LLD_CmdSpiOnly, LLD_OK
 */
LLD_STATUS SPI_NOR_EQIO( FlashStatus *fsptr )
{
	FLASH_WRITE(SPI_NOR_CMD_EQIO,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);

    // Set QPI enable flag
    fsptr->ModeReg = ( fsptr->ModeReg | 0x01 );

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_RSTQIO
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The RSTQIO instruction resets the device
 *                 from QPI mode to 1-bit SPI protocol operation.
 * Return Value: LLD_CmdQpiOnly, LLD_OK
 */
LLD_STATUS SPI_NOR_RSTQIO( FlashStatus *fsptr )
{
    // Check current state
    if( !IsFlashQPI( fsptr ) )    return LLD_CmdQpiOnly;
	
	FLASH_WRITE(SPI_NOR_CMD_RSTQIO,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);

    // Reset QPI enable flag
    fsptr->ModeReg = ( fsptr->ModeReg & 0xfe );

    return LLD_OK;
}


/*
 * Function:       SPI_NOR_SBL
 * Arguments:      burstconfig, burst length configuration
 *                 fsptr, pointer of flash status structure
 * Description:    To set the Burst length(SPI MODE)
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_SBL( uint8 burstconfig, FlashStatus *fsptr )
{
    // Check current state
    if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

    FLASH_WRITE(SPI_NOR_CMD_SBL,0,QIO,QIO,&burstconfig,1,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_SBL_QPI
 * Arguments:      burstconfig, burst length configuration
 *                 fsptr, pointer of flash status structure
 * Description:    To set the Burst length(QPI MODE)
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_SBL_QPI( uint8 burstconfig, FlashStatus *fsptr )
{
    // Check current state
    if( !IsFlashQPI( fsptr ) )    return LLD_CmdQpiOnly;

    FLASH_WRITE(SPI_NOR_CMD_SBL_QPI,ADDRESS_NULL,IO_NULL,QIO,&burstconfig,1,fsptr);

    return LLD_OK;
}

/*
 * Function:       SPI_NOR_EN4B
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The EN4B instruction is for entering 4-byte mode.
 * Return Value: LLD_OK, LLD_WriteRegFail
 */
LLD_STATUS SPI_NOR_EN4B( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_EN4B,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);

    if( IsFlash4Byte( fsptr ) )
        return LLD_OK;
    else
        return LLD_WriteRegFail;
}

/*
 * Function:       SPI_NOR_EX4B
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The EX4B instruction is for exiting 4-byte mode.
 * Return Value: LLD_OK, LLD_WriteRegFail
 */
LLD_STATUS SPI_NOR_EX4B( FlashStatus *fsptr )
{
   FLASH_WRITE(SPI_NOR_CMD_EX4B,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);

    if( IsFlash4Byte( fsptr ) )
        return LLD_WriteRegFail;
    else
        return LLD_OK;
}

/*
 * Reset setting Command
 */

/*
 * Function:       SPI_NOR_RSTEN
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    Enable RST command
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RSTEN( FlashStatus *fsptr )
{
   FLASH_WRITE(SPI_NOR_CMD_RSTEN,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
   return LLD_OK;
}

/*
 * Function:       SPI_NOR_RST
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The RST instruction is used as a system (software) reset that
 *                 puts the device in normal operating Ready mode.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_RST( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_RST,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    // Reset current state
	fsptr->addr_4byte_mode = FALSE;
    fsptr->ModeReg = 0x00;

    return LLD_OK;
}




/*
 * Function:       SPI_NOR_GBLK
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The GBLK instruction is for enable the lock protection
 *                 block of the whole chip.
 *                 This instruction is only effective after WPSEL was executed.
 * Return Value: LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_GBLK( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_GBLK,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
	return LLD_OK;
}

/*
 * Function:       SPI_NOR_GBULK
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The GBULK instruction is for disable the lock protection
 *                 block of the whole chip.
 *                 This instruction is only effective after WPSEL was executed.
 * Return Value: LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */
LLD_STATUS SPI_NOR_GBULK( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_GBULK,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
	return LLD_OK;
}
/*
 * Function:       SPI_NOR_BLK
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The BLK instruction is for enable the lock protection of flash_address
 *                 This instruction is only effective after WPSEL was executed.
 * Return Value: LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */

LLD_STATUS SPI_NOR_BLK( uint32 flash_address, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_BLK,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    return LLD_OK;
   
}

/*
 * Function:       SPI_NOR_BULK
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The BULK instruction is for disable the lock protection of flash_address
 *                 This instruction is only effective after WPSEL was executed.
 * Return Value: LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */

LLD_STATUS SPI_NOR_BULK( uint32 flash_address, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;

    // Check flash is busy or not
    if( IsFlashBusy( fsptr ) )    return LLD_FlashBusy;

    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_WRITE(SPI_NOR_CMD_BULK,flash_address,SIO,IO_NULL,NULL,0,fsptr);

    return LLD_OK;
   
}

/*
 * Function:       SPI_NOR_BLK_READ
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The BLK_READ instruction is for read lock status of flash_address
 *                 This instruction is only effective after WPSEL was executed.
 * Return Value: LLD_FlashBusy, LLD_OK, LLD_TimeOut
 */

LLD_STATUS SPI_NOR_BLK_READ( uint8 *lock_status,uint32 flash_address, FlashStatus *fsptr )
{
    // Check flash address
    if( flash_address > FlashSize ) return LLD_AddressInvalid;
  
    // Check 3-byte or 4-byte mode
    fsptr->addr_4byte_mode = IsFlash4Byte( fsptr );

    FLASH_READ(SPI_NOR_CMD_BLK_READ,flash_address,0,SIO,SIO,lock_status,1,fsptr);

    return LLD_OK;
   
}

/*
 * Suspend/Resume Command
 */

/*
 * Function:       SPI_NOR_PGM_ERS_S
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The PGM_ERS_S suspend Sector-Erase, Block-Erase or
 *                 Page-Program operations and conduct other operations.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_PGM_ERS_S( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_PGM_ERS_S,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}

/*
 * Function:       SPI_NOR_PGM_ERS_R
 * Arguments:      fsptr, pointer of flash status structure
 * Description:    The PGM_ERS_R resume Sector-Erase, Block-Erase or
 *                 Page-Program operations.
 * Return Value: LLD_OK
 */
LLD_STATUS SPI_NOR_PGM_ERS_R( FlashStatus *fsptr )
{
    FLASH_WRITE(SPI_NOR_CMD_PGM_ERS_R,ADDRESS_NULL,IO_NULL,IO_NULL,NULL,0,fsptr);
    return LLD_OK;
}

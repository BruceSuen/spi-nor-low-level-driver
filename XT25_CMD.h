/* XT25_CMD.h - SPI and QPI interface command hex code and function prototype */
 
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
#ifndef    __MX25_SPI_NOR_H__
#define    __MX25_SPI_NOR_H__

#include    "XT25_DEF.h"

/*** XT25 series command hex code definition ***/
//ID commands
#define    SPI_NOR_CMD_RDID      0x9F    //RDID (Read Identification)
#define    SPI_NOR_CMD_RES       0xAB    //RES (Read Electronic ID)
#define    SPI_NOR_CMD_REMS      0x90    //REMS (Read Electronic & Device ID)
#define    SPI_NOR_CMD_REMSDIO   0x92    //REMSDIO(Read Electronic & Device ID Dual I/O)
#define    SPI_NOR_CMD_REMSQIO   0x94    //REMSQIO(Read Electronic & Device ID Quad I/O)

//Register commands
#define    SPI_NOR_CMD_WRSR      0x01    //WRSR (Write Status Register1)
#define    SPI_NOR_CMD_RDSR      0x05    //RDSR (Read Status Register1)
#define    SPI_NOR_CMD_WRSR2     0x31    //WRSR (Write Status Register2)
#define    SPI_NOR_CMD_RDSR2     0x35    //RDSR2 (Read Status Register2)
#define    SPI_NOR_CMD_WRSR3     0x11    //WRSR3 (Write Status Register3)
#define    SPI_NOR_CMD_RDSR3     0x15    //RDSR3 (Read Status Register3)

#define    SPI_NOR_CMD_RDEAR     0xC8    //RDEAR (Read Extended Address Register)
#define    SPI_NOR_CMD_WREAR     0xC5    //WREAR (Write Extended Address Register)



//READ commands
#define    SPI_NOR_CMD_READ         0x03    //READ (1 x I/O)
#define    SPI_NOR_CMD_2READ        0xBB    //2READ (2 x I/O)
#define    SPI_NOR_CMD_4READ        0xEB    //4READ (4 x I/O)
#define    SPI_NOR_CMD_FASTREAD     0x0B    //FAST READ (Fast read data).
#define    SPI_NOR_CMD_WRAPQPI      0x0C    // WRAPQPI ( wrap read in QPI mode)
#define    SPI_NOR_CMD_DREAD        0x3B    //DREAD (1In/2 Out fast read)
#define    SPI_NOR_CMD_QREAD        0x6B    //QREAD (1In/4 Out fast read)
#define    SPI_NOR_CMD_1DTRD        0x0D    //1DTRD (Single DT read)
#define    SPI_NOR_CMD_DTRD_WRAPQPI 0x0E    //DTRD_WRAPQPI(DT  wrap read in qpi mode)

#define    SPI_NOR_CMD_2DTRD       0xBD    //2DTRD (Dual DT read)
#define    SPI_NOR_CMD_4DTRD       0xED    //4DTRD (Quad DT read)
#define    SPI_NOR_CMD_RDSFDP      0x5A    //RDSFDP (Read SFDP)
#define    SPI_NOR_CMD_RDUID       0x4B    //RDUID ( Read Unique ID)
#define    SPI_NOR_CMD_RDSECR      0x48    //RDSECR (Read Security Register )


#define    SPI_NOR_CMD_READ4B      0x13    //READ4B (1 x I/O with 4 byte address)
#define    SPI_NOR_CMD_FASTREAD4B  0x0C    //FASTREAD4B (1 x I/O with 4 byte address)
#define    SPI_NOR_CMD_2READ4B     0xBC    //2READ4B (2 x I/O with 4 byte address)
#define    SPI_NOR_CMD_4READ4B     0xEC    //4READ4B (4 x I/O with 4 byte address)
#define    SPI_NOR_CMD_DREAD4B     0x3C    //DREAD4B (1In/2 Out fast read with 4 byte addr)
#define    SPI_NOR_CMD_QREAD4B     0x6C    //QREAD4B (1In/4 Out fast read with 4 byte addr)
#define    SPI_NOR_CMD_4DTRD4B     0xEE    //4DTRD4B (Quad DT read with 4 byte address)

//Program commands
#define    SPI_NOR_CMD_WREN        0x06    //WREN (Write Enable)
#define    SPI_NOR_CMD_WREN_VOL    0x50    //WREN (Write Enable for Volatile Status Register)
#define    SPI_NOR_CMD_WRDI        0x04    //WRDI (Write Disable)

#define    SPI_NOR_CMD_PP          0x02    //PP (page program)
#define    SPI_NOR_CMD_4PP_32H     0x32    //4PP_32H (Quad page program)
#define    SPI_NOR_CMD_4PP         0xC2    //4PP (Quad page program)

#define    SPI_NOR_CMD_PP4B        0x12    //PP4B (page program with 4 byte address)
#define    SPI_NOR_CMD_4PP_32H4B   0x34    //4PP_32H4B (Quad page program with 4 byte address)
#define    SPI_NOR_CMD_4PP4B       0x3E     //4PP4B (Quad page program with 4 byte address)


#define    SPI_NOR_CMD_PPSECR      0x42    //PPSECR (Program Security Register )

//Erase commands
#define    SPI_NOR_CMD_SE       0x20    //SE (Sector Erase)
#define    SPI_NOR_CMD_BE32K    0x52    //BE32K (Block Erase 32kb)
#define    SPI_NOR_CMD_BE       0xD8    //BE (Block Erase)
#define    SPI_NOR_CMD_CE       0x60    //CE (Chip Erase) hex code: 60 or C7

#define    SPI_NOR_CMD_SE4B     0x21    //SE (Sector Erase with 4 byte addr)
#define    SPI_NOR_CMD_BE32K4B  0x5C    //BE32K4B (Block Erase 32kb with 4 byte addr)
#define    SPI_NOR_CMD_BE4B     0xDC    //BE4B (Block Erase with 4 byte address)

#define    SPI_NOR_CMD_ESECR    0x44    //ESECR (Erase Security Register )


//Mode setting commands
#define    SPI_NOR_CMD_DP       0xB9    //DP (Deep Power Down)
#define    SPI_NOR_CMD_RDP      0xAB    //RDP (Release form Deep Power Down)
#define    SPI_NOR_CMD_EQIO     0x38    //EQIO (Enable Quad I/O)
#define    SPI_NOR_CMD_SBL      0x77    //SBL (Set Burst Length), SPI MODE
#define    SPI_NOR_CMD_SBL_QPI  0xC0    //SBL (Set Burst Length), QPI MODE
#define    SPI_NOR_CMD_EN4B     0xB7    //EN4B( Enter 4-byte Mode )
#define    SPI_NOR_CMD_EX4B     0xE9    //EX4B( Exit 4-byte Mode )

//Reset commands
#define    SPI_NOR_CMD_RSTEN     0x66    //RSTEN (Reset Enable)
#define    SPI_NOR_CMD_RST       0x99    //RST (Reset Memory)
#define    SPI_NOR_CMD_RSTQIO    0xFF    //RSTQIO (Reset Quad I/O)

//Security commands
#define    SPI_NOR_CMD_GBLK       0x7E    //GBLK (Gang Block Lock)
#define    SPI_NOR_CMD_GBULK      0x98    //GBULK (Gang Block Unlock)

#define    SPI_NOR_CMD_BLK       0x36    //BLK (Individual Block Lock)
#define    SPI_NOR_CMD_BULK      0x39    //BULK (Individual Block Unlock)
#define    SPI_NOR_CMD_BLK_READ  0x3D    //BLK_READ (Individual Block Lock Status Read)

//Suspend/Resume commands
#ifdef PGM_ERS_0xB0
#define    SPI_NOR_CMD_PGM_ERS_S    0xB0    //PGM/ERS Suspend (Suspends Program/Erase) old: 0xB0
#define    SPI_NOR_CMD_PGM_ERS_R    0x30    //PGM/ERS Erase (Resumes Program/Erase) old: 0x30
#else
#define    SPI_NOR_CMD_PGM_ERS_S    0x75    //PGM/ERS Suspend (Suspends Program/Erase) new: 0x75
#define    SPI_NOR_CMD_PGM_ERS_R    0x7A    //PGM/ERS Erase (Resumes Program/Erase) new: 0x7A
#endif

#define    ADDRESS_NULL   0xFFFFFFFF

// LLD Status
typedef enum {
    LLD_OK,
    LLD_WriteRegFail,
    LLD_TimeOut,
    LLD_FlashBusy,
    LLD_CmdSpiOnly,
    LLD_CmdQpiOnly,
    LLD_QuadNotEnable,
    LLD_AddressInvalid,
    LLD_Cmd3ByteOnly
}LLD_STATUS;



// Flash status structure define
struct sFlashStatus{
    /* Mode Register:
     * Bit  Description
     * -------------------------
     *  7   Reserved
     *  6   Reserved
     *  5   Reserved
     *  4   Reserved
     *  3   Reserved
     *  2   Reserved
     *  1   Reserved
     *  0   QPI mode enable
    */
    uint8    ModeReg;
	/*Byte Description
	* 0:  3bytes Mode
	* 1:  4 bytes Mode
	*/
	uint8   addr_4byte_mode;
	
};

typedef struct sFlashStatus FlashStatus;

/* Basic functions */


/* Utility functions */
BOOL Wait_Flash_Ready( uint32  ExpectTime, FlashStatus *fsptr );
BOOL IsFlashBusy( FlashStatus *fsptr );
BOOL IsFlashQPI( FlashStatus *fsptr );
BOOL IsFlashQIO( FlashStatus *fsptr );
BOOL IsFlash4Byte( FlashStatus *fsptr );
void SPI_NOR_QUAD_ENABLE(FlashStatus * fsptr);
void SPI_NOR_QUAD_DISABLE(FlashStatus * fsptr);
void SPI_NOR_WPS_ENABLE(FlashStatus * fsptr);
void SPI_NOR_WPS_DISABLE(FlashStatus * fsptr);
void SPI_NOR_LOCK_BIT_ENABLE(uint8 BitMask,FlashStatus * fsptr);







/* Flash commands */

/*ID command*/
LLD_STATUS SPI_NOR_RDID( uint32 *Identification, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RES( uint8 *ElectricIdentification, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_REMS( uint16 *REMS_Identification, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_REMS_DIO( uint16 *REMS_Identification, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_REMS_QIO( uint16 *REMS_Identification, FlashStatus *fsptr );

/*status register functions*/

LLD_STATUS SPI_NOR_RDSR( uint8 *StatusReg, FlashStatus *fsptr );
#ifdef SUPPORT_WRSR_2BYTES
LLD_STATUS SPI_NOR_WRSR( uint16 UpdateValue, FlashStatus *fsptr );
#else
LLD_STATUS SPI_NOR_WRSR( uint8 UpdateValue, FlashStatus *fsptr );
#endif
LLD_STATUS SPI_NOR_RDSR2( uint8 *StatusReg, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRSR2( uint8 UpdateValue, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDSR3( uint8 *StatusReg, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRSR3( uint8 UpdateValue, FlashStatus *fsptr );


/*Read functions*/
LLD_STATUS SPI_NOR_RDEAR( uint8 *ExtReg, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WREAR( uint8 UpdateValue, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRLR( uint16 UpdateValue, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDLR( uint16 *LockReg, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDSPB( uint32 flash_address, uint8 *SPBReg, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRSPB( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_ESSPB( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDDPB( uint32 flash_address, uint8 *DPBReg, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRDPB( uint32 flash_address, uint8 UpdateValue, FlashStatus *fsptr );

LLD_STATUS SPI_NOR_READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_2READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4READ( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_DREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_QREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_FASTREAD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRAPRD_QPI( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDSFDP( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDUID(uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_READ_SECR( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );

LLD_STATUS SPI_NOR_READ4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_FASTREAD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_2READ4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4READ4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_DREAD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_QREAD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );

LLD_STATUS SPI_NOR_1DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_2DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4DTRD( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4DTRD4B( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_DTRD_WRAPQPI( uint32 flash_address, uint8 *target_address, uint32 byte_length, FlashStatus *fsptr );



/*Program functions*/
LLD_STATUS SPI_NOR_WREN( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_WRDI( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_PP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4PP( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4PP_32H( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_PP4B( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4PP4B( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_4PP_32H4B( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_PPSECR( uint32 flash_address, uint8 *source_address, uint32 byte_length, FlashStatus *fsptr );

/*Erase functions*/
LLD_STATUS SPI_NOR_SE( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BE32K( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BE( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BE4B( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_CE( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_SE4B( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BE32K4B( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_ESECR( uint32 flash_address, FlashStatus *fsptr );

/*Mode functions*/
LLD_STATUS SPI_NOR_DP( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RDP( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_EQIO( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_SBL( uint8 burstconfig, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_SBL_QPI( uint8 burstconfig, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_EN4B( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_EX4B( FlashStatus *fsptr );

LLD_STATUS SPI_NOR_RSTEN( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RST( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_RSTQIO( FlashStatus *fsptr );

LLD_STATUS SPI_NOR_GBLK( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_GBULK( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BLK( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BULK( uint32 flash_address, FlashStatus *fsptr );
LLD_STATUS SPI_NOR_BLK_READ( uint8 *lock_status,uint32 flash_address, FlashStatus *fsptr );

LLD_STATUS SPI_NOR_PGM_ERS_S( FlashStatus *fsptr );
LLD_STATUS SPI_NOR_PGM_ERS_R( FlashStatus *fsptr );



#endif    /* __XT25_SPI_NOR_H__ */

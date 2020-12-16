# spi-nor-low-level-driver
## 1. Overview

The XT25-series Low Level Driver(LLD) sample codes are useful for programmers to development flash drivers or software applications of XT25-series SPI NOR flash products. The included source codes are ANSI C compliant. Any ANSI C compliant complier can be compiled with these codes. The XT25-series LLD sample codes have been verified under hardware and software  platform with cortex-M7 IP  core. All the contents of LLD sample code are for reference only. It might contain some unknown problems. XTX will not be liable for any claims or damages due to the use of LLD sample codes.



## 2. File structure

The following files will be available after extracting the zipped file：

| File Name          | File description                                             |
| ------------------ | ------------------------------------------------------------ |
| README.md          | Readme file for XTX SPI NOR low level driver                 |
| XT25_CMD.c         | Flash command sequences of flash devices                     |
| XT25_CMD.h         | SPI and QPI interface command hex code and function prototype |
| XT25_DEF.h         | Definitions of flash devices and program parameters.         |
| XT25_TYPE.h        | Type definition                                              |
| XT25_HAL_EXAMPLE.c | Hardware Abstraction Layer example Code                      |
| XT25_HAL_EXAMPLE.h | Hardware Abstraction Layer example Code head file            |



## 3. Functions

The XT25-series SPI flash contains special I/O transfer modes besides traditional SPI mode. These modes are QPI and DTR mode. LLD source codes 

are different between these modes even if the same flash command. Types of function of LLD are introduced at section 3.1 and command mapping at section 3.2.

### 3.1 Types of Function: There are three types of function for LLD as follows:

####  a. Basic function:

Include CS_Low(), CS_High(), InsertDummyCycle(), SendByte() and GetByte(). Basic functions are highly hardware dependent. You usually need to modify the functions for different hardwares applications. Please see section 4 for details.

#### b. Command function:

Command function is the major part of LLD. It is used to describe the command sequences of flash commands defined in the datasheet.
Normally, the sequences from LLD and datasheet are fully matched. However, please follow the datasheet if any discrepancies. Naming rules of command function are as follows:
```
LLD_STATUS SPI_NOR_XXX(...);
```

Where XXX is a command name like RDID, READ, and so on. LLD_STATUS report the execution result of the command. Section 5 describes more about the return messages.

#### c. Utility function:

Those commands which are neither basic nor command functions belong to utility functions. Utility function provides some unique functionality to make  LLD work smoothly.

### 3.2 Command mapping

As mentioned at section 3.1.b, the command functions are implementation of  command sequences defined in the datasheet. In this section, the mapping of command sequence and command function is introduced. Here takes the RDID command as a example.
```
a. RDID command sequence:
CS# goes low-> send RDID instruction code-> 24-bits ID data out on SO->
to end RDID operation can drive CS# to high at any time during data out.

b. RDID command function:
LLD_STATUS SPI_NOR_RDID( uint32 *Identification, FlashStatus *fsptr )
{
	uint8 gDataBuffer[3];

	// Check current state: SPI or QPI
	if( IsFlashQPI( fsptr ) )    return LLD_CmdSpiOnly;

		FLASH_READ(SPI_NOR_CMD_RDID,ADDRESS_NULL,0,IO_NULL,SIO,gDataBuffer,sizeof(gDataBuffer),fsptr);
		*Identification = gDataBuffer[0] << 16 | gDataBuffer[1] << 8 | gDataBuffer[2];

	return LLD_OK;
}

HAL_STATUS FLASH_READ(uint8 command, uint32 addr, uint8 dummy,uint8 addr_mode,uint8 data_mode,uint8 *data_buffer,uint32 length,FlashStatus *fsptr)
{
	HAL_STATUS  status = HAL_OK;
	uint32 data_cycle = 0;
	CS_Low();
	//Send command
	if( (fsptr->ModeReg & 0x01) == 0x01 )// QPI Mode
	{
		// set addr_mode & data_mode
		if(addr_mode == DTSIO||addr_mode == DTDIO||addr_mode == DTQIO)
		{
			addr_mode = DTQIO;
		}
		else
		{
			addr_mode = QIO;
		}
		
		if(data_mode == DTSIO||data_mode == DTDIO||data_mode == DTQIO)
		{
			data_mode = DTQIO;
		}
		else
		{
			data_mode = QIO;
		}
		
		 //Send command 
		SendByte(command,QIO);
	}
	else
	{
		//Send command 
		SendByte(command,SIO);
	}
	
	//Send Address
	if((addr != ADDRESS_NULL) && (addr_mode != IO_NULL))
	{
		SendFlashAddr(addr,addr_mode,fsptr);
	}
	
	//Send Dummy cycle
	
	if(dummy!=0)
	{
		InsertDummyCycle(dummy);
	}
	
    //Receive Data

   if(length != 0)
   {
    for(data_cycle = 0;data_cycle < length;data_cycle++)
    {
      *(data_buffer+data_cycle) = GetByte(data_mode);
    }
   }
   	
   CS_High();
   return  status;
}

```

In this example, the procedures of command sequence and command function​are matched. The other sequences and functions are similar to this example.​Feel free to verify the relationship between different commands.

## 4. Porting

The XT25-series LLD sample codes are verified by Cotex-M7 IP core. And it uses Cotex-M7's GPIO ports to connect flash device. With different target hardware platforms, it is necessary to modify the LLD source codes. In this section,porting notices are introduced. And it is divided into data types, definitions of options and function modification for your references.

### 4.1 Data types

For the ability of port LLD to different target system, the length of data type can be modified in the XT25_TYPE.h header file. Please verify ​these data types before applying.

```
Basic data type:

typedef    unsigned long     uint32;    /* 32 bit wide */
typedef    unsigned int      uint16;    /* 16 bit wide */
typedef    unsigned char     uint8;     /* 8 bit wide */
typedef    unsigned char     BOOL;      /* 1 bit wide */
```

### 4.2 Definition options

There are several definition options available based on the target system.
```
System option:
#define    MCUXXX              /* Use XXX as micro-controller */
#define    GPIO_SPI            /* Use GPIO port to connect flash */
#define    XT25XXX             /* Flash part name */

MCU option:
#define    CLK_PERIOD          /* MCU clock speed */
#define    Min_Cycle_Per_Inst  /* Cycle count per instruction */
#define    One_Loop_Inst       /* Instruction count per loop */
```

### 4.3 Function modifications

As mentioned at section 3.1, basic functions are highly dependent on the hardwares. And some utility functions work the same. These functions
​should be modified based on different hardware architectures. They are displayed as follows:
```
void CS_Low();
void CS_High();
void InsertDummyCycle( uint8 dummy_cycle);
void SendByte( uint8 byte_value, uint8 transfer_type );
uint8 GetByte( uint8 transfer_type );
void Wait_Flash_WarmUp();
void Initial_Spi();
```

## 5. Return messages
The command functions return a message for operation result. These messages are helpful for programmers to debug at development stage.

```
LLD_STATUS SPI_NOR_XXX( ... );

XXX:
Flash command name. Like RDID, READ, RDSR, and so on.

ReturnMsg:
-FlashOperationSuccess
 Flash command execute successful.

-FlashWriteRegFailed
 Write security register failed or enable block protected mode failed.

-FlashTimeOut
 The time period of write register, program or erase data exceed expected
 time period.

-FlashIsBusy
 The flash device is busy. Waiting WIP bit clear before next write command.

-FlashCmdSpiOnly
 This flash command is supported in SPI mode only.

-FlashCmdQpiOnly
 This flash command is supported in QPI mode only.

-FlashQuadNotEnable
 This flash command needs to set to QE bit before executing.

-FlashAddressInvalid
 The input address exceeds flash's address space.
```

/* XT25_DEF.h -Flash device information define */
 
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

#ifndef    __XT25_DEF_H__
#define    __XT25_DEF_H__

#include <stdio.h>
#include "XT25_TYPE.h"

/*
  Compiler Option
*/


#define    MCU_XXX 1

/* Select your flash device type */
#define XT25F256BXXX
//#define XT25Q128DXXX
//#define XT25Q64DXXX
//#define XT25Q16DXXX
//#define XT25Q08DXXX
//#define XT25F16FXXX





/*
  Flash ID, Timing Information Define
  (The following information could get from device specification)
*/



#define    FLASH_QE_MASK           0x02
#define    FLASH_WPS_MASK          0x40
#define    FLASH_LOCK_BIT_MASK(N)  (0x08 << N)
#define    FLASH_4BYTE_MASK         0x01
#define    tVSL                     10000000       // 10ms 


#ifdef XT25F256BXXX

#define    FlashID          0x0B4019
#define    ElectronicID     0x18
#define    RESID0           0x0B18
#define    RESID1           0x180B
#define    FlashSize        0x2000000      // 32 MB
#define    CE_period        700000000      // 210s, = tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    tW               20000000       // 20ms
#define    tPP              750000         // 0.75ms
#define    tSE              400000000      // 400ms
#define    tBE32            1000000000     // 1s
#define    tBE              1500000000     // 1.5s
#define    tWREAW           40             // 40ns
   
#define    SUPPORT_4BYTE_MODE    
//#define    SUPPORT_WRSR_2BYTES  

#endif

#ifdef XT25Q128DXXX

#define    FlashID          0x0B6018
#define    ElectronicID     0x17
#define    RESID0           0x0B17
#define    RESID1           0x170B
#define    FlashSize        0x1000000      // 16MB
#define    CE_period        700000000      // 210s, = tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    tW               20000000       // 20ms
#define    tPP              750000         // 0.75ms
#define    tSE              400000000      // 400ms
#define    tBE32            1000000000     // 1s
#define    tBE              1200000000     // 1.2s

#endif

#ifdef XT25Q64DXXX

#define    FlashID          0x0B6017
#define    ElectronicID     0x16
#define    RESID0           0x0B16
#define    RESID1           0x160B
#define    FlashSize        0x800000      // 8MB
#define    CE_period        700000000      // 210s, = tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    tW               20000000       // 20ms
#define    tPP              750000         // 0.75ms
#define    tSE              300000000      // 300ms
#define    tBE32            1000000000     // 1s
#define    tBE              1200000000     // 1.2s

#endif


#ifdef XT25Q16DXXX

#define    FlashID          0x0B6015
#define    ElectronicID     0x14
#define    RESID0           0x0B14
#define    RESID1           0x140B
#define    FlashSize        0x200000      // 2MB
#define    CE_period        700000000      // 210s, = tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    tW               10000000       // 10ms
#define    tPP              1000000         // 1ms
#define    tSE              700000000      // 700ms
#define    tBE32            2000000000     // 2s
#define    tBE              4000000000     // 4s

#endif


#ifdef XT25Q08DXXX

#define    FlashID          0x0B6014
#define    ElectronicID     0x13
#define    RESID0           0x0B13
#define    RESID1           0x130B
#define    FlashSize        0x100000      // 1MB
#define    CE_period        700000000      // 210s, = tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    tW               10000000       // 10ms
#define    tPP              1000000         // 1ms
#define    tSE              700000000      // 700ms
#define    tBE32            2000000000     // 2s
#define    tBE              4000000000     // 4s

#endif


#ifdef XT25F16FXXX

#define    FlashID          0x0B4015
#define    ElectronicID     0x14
#define    RESID0           0x0B14
#define    RESID1           0x140B
#define    FlashSize        0x200000      // 2MB
#define    CE_period        700000000      // 210s, = tCE /  ( CLK_PERIOD * Min_Cycle_Per_Inst * One_Loop_Inst)
#define    tW               30000000       // 30ms
#define    tPP              2000000         // 2ms
#define    tSE              300000000      // 300ms
#define    tBE32            1200000000     // 1.2s
#define    tBE              1600000000     // 1.6s

#endif









// variable
#define    TRUE     1
#define    FALSE    0

// Support I/O mode
#define    SIO              0
#define    DIO              1
#define    QIO              2
#define    DTSIO            4
#define    DTDIO            5
#define    DTQIO            6 
#define    IO_NULL          0xFF


/*
  Flash Related Parameter Define
*/

#define    Block_Offset       0x10000     // 64K Block size
#define    Block32K_Offset    0x8000      // 32K Block size
#define    Sector_Offset      0x1000      // 4K Sector size
#define    Page_Offset        0x0100      // 256 Byte Page size
#define    Block_Num          (FlashSize / Block_Offset)

// Flash control register mask define
#define    FLASH_WIP_MASK          0x01


#ifdef MCU_XXX
#define    CLK_PERIOD                1 // unit: ns
#define    Min_Cycle_Per_Inst        1 // cycle count of one instruction
#define    One_Loop_Inst             10 // instruction count of one loop (estimate)
#else
//--- insert your MCU information ---//
#define    CLK_PERIOD                // unit: ns
#define    Min_Cycle_Per_Inst        // cycle count of one instruction
#define    One_Loop_Inst             // instruction count of one loop (estimate)

#endif  //end MCU_XXX

#define    ONE_LOOP_TIME            (CLK_PERIOD*Min_Cycle_Per_Inst*One_Loop_Inst)


// Flash information define
#define    WriteStatusRegCycleTime     tW / ONE_LOOP_TIME
#define    PageProgramCycleTime        tPP / ONE_LOOP_TIME
#define    SectorEraseCycleTime        tSE / ONE_LOOP_TIME
#define    BlockEraseCycleTime         tBE / ONE_LOOP_TIME
#define    ChipEraseCycleTime          CE_period
#define    FlashFullAccessTime         tVSL / ONE_LOOP_TIME


#ifdef tWSR
#define    WriteSecuRegCycleTime       tWSR / ONE_LOOP_TIME
#endif
#ifdef tBE32
#define    BlockErase32KCycleTime      tBE32 / ONE_LOOP_TIME
#endif
#ifdef tWREAW
#define    WriteExtRegCycleTime        tWREAW / ONE_LOOP_TIME
#endif
#endif    /* end of __XT25_DEF_H__  */


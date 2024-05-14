/*
* Configuration of module: NvM
*
* Created by:   parai          
* Copyright:    (C)parai@foxmail.com  
*
* Configured for (MCU):    MinGW ...
*
* Module vendor:           ArcCore
* Generator version:       2.0.34
*
* Generated by easySAR Studio (https://github.com/parai/OpenSAR)
* Current only for NvM+Fee.
*/
#include "NvM.h"
#include "Fee.h"
#include "Ea.h"


NvM_BlockTime_DataGroupType NvM_BlockTime_DataGroup_RAM;
const NvM_BlockTime_DataGroupType NvM_BlockTime_DataGroup_ROM={
	._Year=2013,
	._Month=12,
	._Day=15,
	._Hour=19,
	._Minute=49,
	._Second=0,
};


NvM_BlockConfig_DataGroupType NvM_BlockConfig_DataGroup_RAM;
const NvM_BlockConfig_DataGroupType NvM_BlockConfig_DataGroup_ROM={
	._Byte={0,0},
};


NvM_BlockFingerPrint_DataGroupType NvM_BlockFingerPrint_DataGroup_RAM;
const NvM_BlockFingerPrint_DataGroupType NvM_BlockFingerPrint_DataGroup_ROM={
	._Data={0,0},
};


NvM_BlockEaTest1_DataGroupType NvM_BlockEaTest1_DataGroup_RAM;
const NvM_BlockEaTest1_DataGroupType NvM_BlockEaTest1_DataGroup_ROM={
	._Data0=1,
	._Data1=2,
	._Data2=3,
	._Data3=4,
};


NvM_BlockEaTest2_DataGroupType NvM_BlockEaTest2_DataGroup_RAM;
const NvM_BlockEaTest2_DataGroupType NvM_BlockEaTest2_DataGroup_ROM={
	._Data1=5,
	._Data2=6,
	._Data3=7,
	._Data4=8,
};

const NvM_BlockDescriptorType BlockDescriptorList[] = {

    {
        .BlockManagementType = NVM_BLOCK_NATIVE,
        .SelectBlockForReadall = TRUE,
        .SingleBlockCallback = NULL,
        .NvBlockLength        = 7,
        .BlockUseCrc  = TRUE,
        .BlockCRCType =NVM_CRC16,
        .RamBlockDataAddress = (uint8*)&NvM_BlockTime_DataGroup_RAM,
        .CalcRamBlockCrc = FALSE, // TODO
        .NvBlockNum = FEE_BLOCK_NUM_Time,
        .NvramDeviceId = FEE_INDEX,
        .NvBlockBaseNumber = FEE_BLOCK_NUM_Time,
        .InitBlockCallback = NULL,
        .RomBlockDataAdress = (uint8*)&NvM_BlockTime_DataGroup_ROM,
    },

    {
        .BlockManagementType = NVM_BLOCK_NATIVE,
        .SelectBlockForReadall = TRUE,
        .SingleBlockCallback = NULL,
        .NvBlockLength        = 32,
        .BlockUseCrc  = TRUE,
        .BlockCRCType =NVM_CRC16,
        .RamBlockDataAddress = (uint8*)&NvM_BlockConfig_DataGroup_RAM,
        .CalcRamBlockCrc = FALSE, // TODO
        .NvBlockNum = FEE_BLOCK_NUM_Config,
        .NvramDeviceId = FEE_INDEX,
        .NvBlockBaseNumber = FEE_BLOCK_NUM_Config,
        .InitBlockCallback = NULL,
        .RomBlockDataAdress = (uint8*)&NvM_BlockConfig_DataGroup_ROM,
    },

    {
        .BlockManagementType = NVM_BLOCK_NATIVE,
        .SelectBlockForReadall = TRUE,
        .SingleBlockCallback = NULL,
        .NvBlockLength        = 256,
        .BlockUseCrc  = TRUE,
        .BlockCRCType =NVM_CRC16,
        .RamBlockDataAddress = (uint8*)&NvM_BlockFingerPrint_DataGroup_RAM,
        .CalcRamBlockCrc = FALSE, // TODO
        .NvBlockNum = FEE_BLOCK_NUM_FingerPrint,
        .NvramDeviceId = FEE_INDEX,
        .NvBlockBaseNumber = FEE_BLOCK_NUM_FingerPrint,
        .InitBlockCallback = NULL,
        .RomBlockDataAdress = (uint8*)&NvM_BlockFingerPrint_DataGroup_ROM,
    },

    {
        .BlockManagementType = NVM_BLOCK_NATIVE,
        .SelectBlockForReadall = TRUE,
        .SingleBlockCallback = NULL,
        .NvBlockLength        = 16,
        .BlockUseCrc  = TRUE,
        .BlockCRCType =NVM_CRC16,
        .RamBlockDataAddress = (uint8*)&NvM_BlockEaTest1_DataGroup_RAM,
        .CalcRamBlockCrc = FALSE, // TODO
        .NvBlockNum = EA_BLOCK_NUM_EaTest1,
        .NvramDeviceId = EA_INDEX,
        .NvBlockBaseNumber = EA_BLOCK_NUM_EaTest1,
        .InitBlockCallback = NULL,
        .RomBlockDataAdress = (uint8*)&NvM_BlockEaTest1_DataGroup_ROM,
    },

    {
        .BlockManagementType = NVM_BLOCK_NATIVE,
        .SelectBlockForReadall = TRUE,
        .SingleBlockCallback = NULL,
        .NvBlockLength        = 16,
        .BlockUseCrc  = TRUE,
        .BlockCRCType =NVM_CRC16,
        .RamBlockDataAddress = (uint8*)&NvM_BlockEaTest2_DataGroup_RAM,
        .CalcRamBlockCrc = FALSE, // TODO
        .NvBlockNum = EA_BLOCK_NUM_EaTest2,
        .NvramDeviceId = EA_INDEX,
        .NvBlockBaseNumber = EA_BLOCK_NUM_EaTest2,
        .InitBlockCallback = NULL,
        .RomBlockDataAdress = (uint8*)&NvM_BlockEaTest2_DataGroup_ROM,
    },
};

const NvM_ConfigType NvM_Config = {
    .Common = {
        .MultiBlockCallback = NULL,
    },
    .BlockDescriptor = BlockDescriptorList,        
};


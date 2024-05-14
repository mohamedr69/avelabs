
#ifndef FEE_CFG_H_
#define FEE_CFG_H_

#include "MemIf_Types.h"
#include "Fee_ConfigTypes.h"

#define FEE_DEV_ERROR_DETECT			STD_ON
#define FEE_VERSION_INFO_API    		STD_ON
#define FEE_POLLING_MODE				STD_ON

#define FEE_INDEX						0
#define FEE_VIRTUAL_PAGE_SIZE		    8

#define FEE_BLOCK_NUM_SYSTEM_TIME       1    // size is 8, occupy 1 and 2
#define FEE_BLOCK_NUM_TEST1             3    // size is 4
#define FEE_BLOCK_NUM_TEST2             4	 // size is 4

#define FEE_NUM_OF_BLOCKS				3
#define FEE_MAX_NUM_SETS				1

#endif /*FEE_CFG_H_*/

#include "stub.h"
.text
ARM32_FN(0x40014174,printf)
//kernel 
ARM32_FN(0x40022698,loadkernel) //bootloader
ARM32_FN(0x40014300,runkernel) //bootloader

ARM32_FN(0x407760a4,TaskEntry) //branched code

ARM32_FN(0x40536408,assert) //kernel

//I2C stuff
ARM32_FN(0x404bb23c,VCCOnPower);
ARM32_FN(0x404bb254,VCCOffPower);
ARM32_FN(0x404e804c,writeI2C);
ARM32_FN(0x404e8268,applyI2C);

//Button
ARM32_FN(0x4000DEE0,thing);

//Tasks
ARM32_FN(0x40536b04,OSAL_CreateTask)
ARM32_FN(0x40539b30,OSAL_SeizeSemaphore)
ARM32_FN(0x40539cf4,OSAL_ReleaseSemaphore)
ARM32_FN(0x405375f8,OSAL_YieldTask)
//Misc {not used or maybe used in the future}
ARM32_FN(0x4001e258,flash_write) //do not call without investigation
ARM32_FN(0x4001ec70,flash_load)
ARM32_FN(0x40164054,check_checksum);
//Core functions for hooking and a main task for displaying on the LCD.
//This is intended to run on the modded kernel.

#include "functions.h"
#include "platform/365W/consts.h"
#include "./DisplayLibrary/display.h"
#pragma arm
//#define DEBUG

static int task()
{
  disp_init();
  printf("\n End I2C Display Init\n");
  disp_clear();
  disp_print("Display Init.. Done!", 0, 0);
  disp_print("Custom Code Ver: 0.01", 0, 8);
  disp_print("Written by: turtius", 0, 16);
  disp_print("                      ", 0, 24);
  disp_print("Press any button", 0, 32);
  disp_print("----------------", 0, 40);
  disp_update();
  while (1)
  {
   switch MEM(CURRENT_BUTTON_REG){
   case ERROR_BUTTON_VAL:
      disp_clear();
      disp_print("ERROR BTN", 0, 0);
      disp_update();
   case WPS_BUTTON_VAL:
      disp_clear();
      disp_print("WPS BTN", 0, 0);
      disp_update();
   case POWER_BUTTON_VAL:
      disp_clear();
      disp_print("PWR BTN", 0, 0);
      disp_update();
    //printf("Button1: %x  Button2: %x Button3: %x Button4: %x\n",MEM(0x40a57d10),MEM(0x40a57d14),MEM(0x40a57c74),MEM(0x40a57c6c));
  }
   OSAL_YieldTask(5);
  }
}
static int kernelmain()
{
  printf("\n branched sucessfully. \n");
  printf("\n Executing Tasks\n");
  //Return the actual flow.

  //replace the  string back
  //I'm not sure why changing this changes the printer mode to THUMB and crashing it so MEM macro could not be used here.
  int *word = (int *)0x4077567c;
  *word = 0x416e6368; //Anch
  TaskEntry();
  //Can execute anything underneath.
  //          OSAL_CreateTask(s_AppTmMgr_40533494,PTR_FUN_40533490,0,0x2000,0xcb,PTR_DAT_4053348c,
  //                         s_AppTmMgr_Lib.c_40533408,0x58);
  OSAL_CreateTask("CustomCode", task, 0, 0x2000, 0x40);
  return 1;
}
void main()
{

  printf("Custom code running. \n");
  printf("kernelmain function at: %x\n", &kernelmain);
  printf("Version 0.01 by turtiustrek\n");
  //check_checksum(0x41200000,0x399876+ i);

  MEM(0x4002279c) = 0xe8bd8018; //ldmia sp!,{r3,r4,pc}
  loadkernel();                 //put in memory and return.
  MEM(0x4002279c) = 0xe1a00004; //mov param_1,r4

  //MEM(0x4076a95c) = 0xf0f00000;
  //pc offset calculation =  (dest - curr ins )-8
  //jump hook main loop
  MEM(0x4077567c) = &kernelmain;
  MEM(0x40775570) = 0xe5bff104; //ldr pc, [pc, #0x104]!

  MEM(0x404bb1c0) = 0xe8bd807e; //VCCOffPower return. aka make it say on

#ifdef DEBUG
      //Debugging logs hook MAY CRASH LOL
  MEM(0x405363dc) = &printf;
  MEM(0x40536048) = 0xe5bff38c; //ldr pc, [pc, #0x38c]!

  //NOP spam logs (slows down boot by ALOT)
  //OSAL stuff mainly
  //tonop : ReleaseSemaphore(4),SeizeSeamaphore(3), OSAL_UnlockMutex(3),OSAL_LockMutex(2)
  int tonop[12] = {0x40539d04, 0x40539d7c, 0x40539da0, 0x40539d4c, 0x40539bf8, 0x40539c30, 0x40539c04, 0x4053a490, 0x4053a548, 0x4053a4d4, 0x4053a2f0, 0x4053a3c0};
  for (int i = 0; i < 12; i++)
  {
    printf("To NOP: %x \n", tonop[i]);
    MEM(tonop[i]) = 0xe320f000; //nop
  }
#endif
  runkernel(0x40138000); //Kernel entry.
  return;
}

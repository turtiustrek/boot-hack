/*
Written by turtiustrek with parts from:
https://github.com/dhepper/font8x8

Written for the printer only. 
*/

#include "display.h"
#include "../functions.h"
#include "font.h"
static char buffer[1024] = {0};
static char cmdbuf[2] = {0,0};
// Custom Functions
void disp_queue(int value, int cd)
{
  if (OSAL_SeizeSemaphore(MEM(0x40b2a448), 0, 0, "HAL_I2C.c", 0x13b) != 0)
  {
    assert("Failed to seize I2C semaphore..\n");
  }
  MEM(0x40add0b4) = 0x01000000; //has to be one so the writeI2C function can well function as its a cmp in one the functions in apply
  //horrible implentation but this would work
  
  if (cd == 0)
  {
    cmdbuf[0] = 0x0;
    cmdbuf[1] = value;
  }
  else
  { //treat anything else as data {much safer}
    cmdbuf[0] = 0x40;
    cmdbuf[1] = value;
  }

  writeI2C(0x78, 0x2, cmdbuf);
  if (OSAL_ReleaseSemaphore(MEM(0x40b2a448), "HAL_I2C.c", 0x154) != 0)
  {
    assert("Failed to release I2C semaphore..\n");
  }
}
void disp_update()
{
  for (int i = (64 / 8) - 1; i >= 0; i--)
  {
    disp_queue(0xB0 + i, 0); // Set row
    disp_queue(0x02, 0);     // Set lower column address
    disp_queue(0x10, 0);     // Set higher column address

    for (int j = 128; j > 0; j--)
    {
      // send a bunch of data in one xmission

      for (int x = 0; x < 16; x++)
      {
        disp_queue(buffer[i * 128 + 128 - j], 1);
        j--;
      }
      j++;
    }
  }
  applyI2C();
}
void disp_putpixel(int x, int y, int color)
{
  if ((x >= 128) || (y >= 64))
    return;
  if (color)
    buffer[x + (int)(y / 8) * 128] |= (1 << (y & 7));
  else
    buffer[x + (int)(y / 8) * 128] &= ~(1 << (y & 7));
}
void disp_drawchar(char letter, int xpos, int ypos)
{
  int x, y;
  int set;
  int mask;

  for (x = 0; x < font_height; x++)
  {
    for (int cursor = 0; cursor < font_height; cursor++)
    {
      for (y = 0; y < font_width; y++)
      {
        //Array indexing explained..
        /*
        Since the array contains font_width elements for each char it was needed to multiply it by the letter that we are after
        starting from 0 to 127 (+ the index for the char). In this case though the array was not extracted from 0 to 127 but rather
        32 to 127 which needed to be compensated and that is what the -(32*font_width) is for. The & (1<<x) part is masking the other
        bits so that one bit can be 'read' at a time. 
        */
        set = Ubuntu_Mono5x8[(letter * font_width) + y - (32 * font_width)] & (1 << x);
        //uart_printf("%c", set ? 'X' : ' ');
        disp_putpixel(y + xpos, x + ypos + cursor, set ? 0x1 : 0);
      }
    }
    // uart_printf("\n");
  }
}

void disp_print(char *string, int xpos, int ypos)
{
  int cursor = 0;
  for (int i = 0; string[i] != '\0'; i++)
  {
    disp_drawchar(string[i], cursor + xpos, ypos);
    cursor += font_width;
  }
}

void disp_clear()
{
  for (int i = 0; i < 1024; i++)
  {
    buffer[i] = 0;
  }
}
void disp_init()
{
  VCCOnPower();
  OSAL_YieldTask(1000);
  uart_printf("\n Start I2C Display Init\n");
  //From adafruit library
  // memset(buffer,0,1024); //set buffer to 0
  disp_queue(0xAE, 0); //--turn off oled panel
  applyI2C();
  disp_queue(0x02, 0); //---set low column address
  disp_queue(0x10, 0); //---set high column address
  disp_queue(0x40, 0); //--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  disp_queue(0x81, 0); //--set contrast control register
  disp_queue(0xA0, 0); //--Set SEG/Column Mapping
  disp_queue(0xC0, 0); //Set COM/Row Scan Direction
  disp_queue(0xA6, 0); //--set normal display
  disp_queue(0xA8, 0); //--set multiplex ratio(1 to 64)
  disp_queue(0x3F, 0); //--1/64 duty
  disp_queue(0xD3, 0); //-set display offset    Shift Mapping RAM Counter (0x00~0x3F)
  disp_queue(0x00, 0); //-not offset
  disp_queue(0xd5, 0); //--set display clock divide ratio/oscillator frequency
  disp_queue(0x80, 0); //--set divide ratio, Set Clock as 100 Frames/Sec
  disp_queue(0xD9, 0); //--set pre-charge period
  disp_queue(0xF1, 0); //Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  disp_queue(0xDA, 0); //--set com pins hardware configuration
  disp_queue(0x12, 0);
  disp_queue(0xDB, 0); //--set vcomh
  disp_queue(0x40, 0); //Set VCOM Deselect Level
  disp_queue(0x20, 0); //-Set Page Addressing Mode (0x00/0x01/0x02)
  disp_queue(0x02, 0); //
  disp_queue(0xA4, 0); // Disable Entire Display On (0xa4/0xa5)
  disp_queue(0xA6, 0); // Disable Inverse Display On (0xa6/a7)
  disp_queue(0xAF, 0); //--turn on oled panel
  applyI2C();
}

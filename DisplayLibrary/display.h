#ifndef DISPLAY_H_   /* Include guard */
#define DISPLAY_H_
 void disp_queue(int value,int cd);
 void disp_update();
 void disp_putpixel(int x, int y, int color);
 void disp_drawchar(char letter,int xpos,int ypos);
 void disp_print(char *string,int xpos, int ypos);
 void disp_clear();
 void disp_init();
#endif //DISPLAY_H_



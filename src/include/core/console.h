#ifndef __CONSOLE_H__
#define __CONSOLE_H_
#include <core/arch_x86/typedef.h>

// return the current x,y position if the cursor
// on the monitor
void getxy(u8int *x1,u8int *y1);

// Update the postion of the hardware cursor
void update_cursor();

// Set the foreground,background color of charecter
// printed after this function is called. It will
// not effect previous char
void setcolor(u8int color);

// Set the x,y position of the cursor in the monitor
// which will effect the where the next character will
// be printed after this function is called
void setxy(u8int new_x,u8int new_y);

// Clear everything in the screen by overwritting ' ' Space 
// all over the screen
void clrscr();

// This move every line above and clear the last line
void scroll();

// put a char on the screen after the previous one
void putchar(s8int ch);

// Print a null trminated char array on the screen
void puts(s8int *msg);

// Convert a Integer into Char array and ptint it to 
// screen
void putint(int no);

// Print the Integer into Hex format
void putint_hex(int no);
#endif

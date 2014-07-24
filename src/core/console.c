#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/console.h>


extern u8int graphical_mode;


u8int x = 0,y = 0; // x,y position in the screen
u8int attrib = 0x9f; // foreground & background
u16int *txtmem = (u16int*)0xb8000;

// return the current x,y position if the cursor
// on the monitor
void getxy(u8int *x1,u8int *y1)
{
	*x1 = x;
	*y1 = y;
}

// Update the postion of the hardware cursor
void update_cursor()
{
	if(!graphical_mode)
	{
		u8int temp = y*80 + x;
		outb(0x3D4, 14);
		outb(0x3D5, temp >> 8);
    	outb(0x3D4, 15);
    	outb(0x3D5, temp);
	}
}

// Set the foreground,background color of charecter
// printed after this function is called. It will
// not effect previous char
void setcolor(u8int color)
{
	attrib = color;
}

// Set the x,y position of the cursor in the monitor
// which will effect the where the next character will
// be printed after this function is called
void setxy(u8int new_x,u8int new_y)
{
	x = new_x;
	y = new_y;
	update_cursor();
}

// Clear everything in the screen by overwritting ' ' Space 
// all over the screen
void clrscr()
{
	if(!graphical_mode)
	{
		u16int *dest = txtmem;
		u16int lineno = 0;
		while(lineno < 25*80)
		{
			*dest++ = attrib<<8| ' ';
			lineno++;
		}
	}
	setxy(0,0);
	update_cursor();
}

// This move every line above and clear the last line
void scroll()
{
	if(!graphical_mode)
	{
		u16int *dest = txtmem;
		u16int *src = &txtmem[80];
		u8int lineno = 0;
		u8int i;
		while(lineno < 25)
		{
			for(i=0;i<80;i++)
			{
				*dest++ = *src++;
			}
			lineno++;
		}
		// clear last line
		for(i=0;i<80;i++)
		{
			*dest++ = attrib<<8| ' ';
		}
		setxy(0,24);
	}
	update_cursor();
}

// put a char on the screen after the previous one
void putchar(s8int ch)
{
	if(!graphical_mode)
	{
		if(y >= 25) scroll();
		if(ch == '\b')
		{
			if(x+y > 0) // see if there is any ch to erase
			{
				txtmem[(y*80 + x)-1] = attrib<<8|0x20;
				if(x==0)
				{
					x = 25;
					y--;
				}
				else
				{
					x--;
				}
			}
		}
		else if(ch == '\t')
		{
			x += 4;
		}
		else if(ch == '\n')
		{
			x = 0;
			y++;
		}
		else if(ch == '\r')
		{
			x = 0;
		}
		else
		{
			txtmem[(y*80 + x)] = attrib<<8|ch;
			x++;
		}
	}
	if(x >= 80) x=80-x,y++;
	update_cursor();
}




// Print a null trminated char array on the screen
void puts(s8int *msg)
{

	while(*msg != '\0')
		putchar(*msg++);
	update_cursor();
}

// Convert a Integer into Char array and ptint it to 
// screen
void putint(int no)
{
	char txt[10] = "";
	itoa(txt,'d',no);
	puts(txt);
	update_cursor();
}

// Print the Integer into Hex format
void putint_hex(int no)
{
	char txt[10] = "";
	itoa(txt,'x',no);
	puts("0x");
	puts(txt);
	update_cursor();
}




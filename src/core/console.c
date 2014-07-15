#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/console.h>


extern u8int graphical_mode;


u8int x = 0,y = 0; // x,y position in the screen
u8int attrib = 0x0F; // foreground & background
u16int *txtmem = (u16int*)0xb8000;


// Update the postion of the hardware cursor 
void update_cursor()
{
	if(!graphical_mode)
	{
		u8int temp = y*80 + x;
		 /* This sends a command to indicies 14 and 15 in the
   		  *  CRT Control Register of the VGA controller. These
  		  *  are the high and low bytes of the index that show
  		  *  where the hardware cursor is to be 'blinking'. To
  		  *  learn more, you should look up some VGA specific
  		  *  programming documents. A great start to graphics:
 	      *  http://www.brackeen.com/home/vga 
 	      */
		outb(0x3D4, 14);
  		outb(0x3D5, temp >> 8);
    	outb(0x3D4, 15);
    	outb(0x3D5, temp);
	}
}

// set the pointer
void setxy(u8int new_x,u8int new_y)
{
	x = new_x;
	y = new_y;
	update_cursor();
}

/*
void clrscr()
{
	if(!graphical_mode)
	{
		u16int *dest = txtmem;
		u16int lineno = 0;
		while(lineno < 25*80)
		{
			*dest++ = attrib<<8| '.';
			lineno++;
		}
	}

}
*/

void clrscr()
{
	int i = 0;
	setxy(0,0);
	while(i < 80*25)
	{
		putchar(' ');
		i++;
	}
	setxy(0,0);
}


// scroll the screen one line above
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
			*dest = attrib<<8| ' ';
		}
		setxy(0,24);
	}
}

// put a char on the screen after the previous one
void putchar(u8int ch)
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

// Print a String 
void puts(u8int *msg)
{
	while(*msg != '\0')
		putchar(*msg++);
}




#include <system.h>

u8int graphical_mode = 0;

void kernel_start(multiboot_header_t *mbd,u32int initial_stack)
{
	clrscr();
	puts("Hello World");
	int i;
	char txt[10] = "";
	
	for(i=0;i<26;i++)
	{
		parseint(i,txt);
		puts(txt);
		putchar('\n');
	}
	
	

	// don't remove this
	// never ever
	// if you still dare kernel will curse
	// you with strange behaviour which you
	// not able to debug and 
	for(;;);
}

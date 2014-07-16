#include <system.h>

u8int graphical_mode = 0;

void kernel_start(multiboot_header_t *mbd,u32int initial_stack)
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	keyboard_install();
	
	__asm__ __volatile__ ("sti"); 
	initialise_paging();
	clrscr();
	puts("Paging Enabled");
	//u32int *ptr = (u32int*)0xa0000000;
	

	// don't remove this
	// never ever
	// if you still dare kernel will curse
	// you with strange behaviour which you
	// not able to debug and 
	puts("hangon");
	for(;;);
	puts("hang-off");
}

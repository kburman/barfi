#include <system.h>

u8int graphical_mode = 0;

void kernel_start(multiboot_header_t *mbd,u32int initial_stack)
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	timer_install();
	__asm__ __volatile__ ("sti"); 
	
	clrscr();
	datetime_t dt;	
	char txt[10] ="";
	
	for(;;)
	{
		setxy(64,0);
		dt = getDatetime();
		parseint(dt.year,txt);
		puts(txt);
	
		parseint(dt.month,txt);
		puts("/");
		puts(txt);
	
		parseint(dt.day,txt);
		puts("/");
		puts(txt);
	
		parseint(dt.hour,txt);
		puts(" ");
		puts(txt);
	
		parseint(dt.min,txt);
		puts(":");
		puts(txt);
	
		parseint(dt.sec,txt);
		puts(":");
		puts(txt);
		timer_wait(100);
	}
	
	

	// don't remove this
	// never ever
	// if you still dare kernel will curse
	// you with strange behaviour which you
	// not able to debug and 
	puts("hangon");
	for(;;);
	puts("hang-off");
}

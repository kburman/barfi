#include <system.h>
#define debug_kernel
u8int graphical_mode = 0;
extern u32int *end;
multiboot_header_t *ptr;

void kernel_start(multiboot_header_t *mbd,u32int initial_stack)
{
	ptr = mbd;
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	init_mm(1024*18*1024); // 18mb ram
	timer_install();
	keyboard_install();
	__asm__ __volatile__ ("sti");

	#ifdef debug_kernel
		printDetails();
	#endif // debug
	for(;;);
	puts("hang-off");
}

#ifdef debug_kernel
void printDetails()
{

	setcolor(0x0f);
	puts("Kernel Starts at : ");setcolor(0x03);putint((int)kernel_start);putchar('\n');

	setcolor(0x0f);
	puts("Ram  Size : ");setcolor(0x03);putint((ptr->mem_upper));putchar('\n');

	setcolor(0x0f);
	puts("Command Line : ");setcolor(0x03);puts(ptr->cmdline);putchar('\n');

	setcolor(0x0f);
	puts("Bootloader name : ");setcolor(0x03);puts(ptr->boot_loader_name);putchar('\n');
}

#endif // debug


































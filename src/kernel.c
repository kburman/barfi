#include <system.h>
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))
u8int graphical_mode = 0;	// 0 =Console ,1 = OMG we are in Graphical UI
extern u32int *end;
void kernel_start(multiboot_header_t *mbd,u32int initial_stack)
{
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	inti_mm();	// memory manger
	timer_install();
	keyboard_install();
	__asm__ __volatile__ ("sti");


	printStats();

	// find RAM
	u32int mem_space = mbd->mem_upper - mbd->mem_lower;
	char txt[10] = "";

	itoa(txt,10,mem_space);
	puts("RAM = ");
	puts(txt);
	putchar('\n');

	mem_space = mbd->mem_upper;
	itoa(txt,10,mem_space);
	puts("Upper Ram = ");
	puts(txt);
	putchar('\n');

	mem_space = mbd->mem_lower;
	itoa(txt,10,mem_space);
	puts("Lower Ram = ");
	puts(txt);
	putchar('\n');


	for(;;);
	puts("hang-off");
}

//void testkmalloc()
//{
//	char txt[10] = "";
//	char *ptr;
//
//	ptr = (char*)kmalloc(1);
//	itoa(txt,10,(u32int)ptr);
//	puts("end = ");
//	puts(txt);
//	putchar('\n');
//
//	ptr = (char*)kmalloc(1);
//	itoa(txt,10,(u32int)ptr);
//	puts("end = ");
//	puts(txt);
//	putchar('\n');
//}



void printStats()
{
	char txt[10] = "";
	u32int *ptr;

	itoa(txt,10,(int)&kernel_start);
	puts("kernel_start at : ");
	puts(txt);
	putchar('\n');

	// print the end location
	itoa(txt,10,(u32int)&end);
	puts("end = ");
	puts(txt);
	putchar('\n');
}

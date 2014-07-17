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

void testkmalloc()
{
	char txt[10] = "";
	char *ptr;

	ptr = (char*)kmalloc(1);
	itoa(txt,10,(u32int)ptr);
	puts("end = ");
	puts(txt);
	putchar('\n');

	ptr = (char*)kmalloc(1);
	itoa(txt,10,(u32int)ptr);
	puts("end = ");
	puts(txt);
	putchar('\n');
}



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

	ptr = (char*)kmalloc(7);
	itoa(txt,10,(int)ptr);
	puts("allocated 7 bytes at : ");
	puts(txt);
	putchar('\n');

	ptr = (char*)kmalloc(16);
	itoa(txt,10,(int)ptr);
	puts("allocated 16 bytes at : ");
	puts(txt);
	putchar('\n');


	// some test number
	puts("\n\n\n");
	u32int *frames;
	u32int nframes;
	u32int mem_end_page = 0x1000000;
    nframes = mem_end_page / 0x1000;

    itoa(txt,10,nframes);
	puts("nframes : ");
	puts(txt);
	putchar('\n');

	itoa(txt,10,mem_end_page);
	puts("Memory end : ");
	puts(txt);
	putchar('\n');


	nframes = INDEX_FROM_BIT(nframes);
	itoa(txt,10,nframes);
	puts("INDEX_FROM_BIT(nframes) : ");
	puts(txt);
	putchar('\n');


	itoa(txt,10,sizeof(page_directory_t));
	puts("sizeof page_directory_t : ");
	puts(txt);
	putchar('\n');
}

#include <system.h>
u8int graphical_mode = 0;
extern u32int *end;
multiboot_header_t *mbd_ptr;
u32int stack_start; // stack grows downward

void kernel_start(multiboot_header_t *mbd,u32int initial_stack)
{
	clrscr();
	gdt_install();
	idt_install();
	isrs_install();
	irq_install();
	init_mm(1024*20*1024); // 20mb ram
	init_paging();
	timer_install();
	keyboard_install();
	__asm__ __volatile__ ("sti");
	mbd_ptr = mbd;
	stack_start = initial_stack;
	//test_kmalloc()  ;
	start_terminal();

	for(;;);
	puts("hang-off");
}



void test_kmalloc()
{
	physical_address ad0;
	ad0 = alloc_block();
	setcolor(0x0f);
	puts("phy0 :");setcolor(0x03);putint((int)ad0);putchar('\n');

	physical_address ad1;
	ad1 = alloc_block();
	setcolor(0x0f);
	puts("phy1 :");setcolor(0x03);putint((int)ad1);putchar('\n');

	physical_address ad2;
	ad2 = alloc_block();
	setcolor(0x0f);
	puts("phy2 :");setcolor(0x03);putint((int)ad2);putchar('\n');

	dealloc_block(ad1);
	setcolor(0x09);
	puts("dealoc-phy1 :");setcolor(0x03);putint((int)ad1);putchar('\n');

	physical_address ad3;
	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy3 :");setcolor(0x03);putint((int)ad3);putchar('\n');

	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy4 :");setcolor(0x03);putint((int)ad3);putchar('\n');

	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy5 :");setcolor(0x03);putint((int)ad3);putchar('\n');

	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy6 :");setcolor(0x03);putint((int)ad3);putchar('\n');

	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy7 :");setcolor(0x03);putint((int)ad3);putchar('\n');

	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy8 :");setcolor(0x03);putint((int)ad3);putchar('\n');

	dealloc_block(ad2);
	setcolor(0x09);
	puts("dealoc-phy2 :");setcolor(0x03);putint((int)ad2);putchar('\n');

	ad3 = alloc_block();
	setcolor(0x0f);
	puts("phy8 :");setcolor(0x03);putint((int)ad3);putchar('\n');
}
































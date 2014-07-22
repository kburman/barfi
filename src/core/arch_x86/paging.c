#include <core/arch_x86/typedef.h>
#include <core/arch_x86/kmalloc.h>
#include <core/arch_x86/regs.h>
#include <core/arch_x86/irq.h>

u32int *kernel_dir;
u32int *first_table;
extern void enablePaging();

void pagefault_handler(struct regs *r)
{
	puts("hey it is Page fault1");
}

void init_paging()
{
	int i,address;
	
	kernel_dir = (u32int*)alloc_block();
	first_table= (u32int*)alloc_block();
		
	memset((u8int*)kernel_dir,0,4096);
	memset((u8int*)first_table,0,4096);
	
	for(i=0;i<1024;i++)
	{
		kernel_dir[i] = 0 | 2;
	}
	address = 0;
	for(i=0;address<findFirst();i++)
	{
		first_table[i] = address | 3;
		address = address + 4096;
	}	
	
	kernel_dir[0] = first_table;
	kernel_dir[0] |= 3;
	irq_install_handler(14, pagefault_handler);
    switch_page_directory(&kernel_dir[0]);
    //enablePaging();
}


void switch_page_directory(u32int *dir)
{
  asm volatile("mov %0, %%cr3":: "r"(&dir[0]));
  u32int cr0;
  asm volatile("mov %%cr0, %0": "=r"(cr0));
  cr0 |= 0x80000000; // Enable paging!
  asm volatile("mov %0, %%cr0":: "r"(cr0));
}

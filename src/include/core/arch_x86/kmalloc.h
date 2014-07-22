#ifndef __KMALLOC_H__
#define __KMALLOC_H__
#include <core/arch_x86/typedef.h>


void init_mm(u32int ram_sz_bytes);
physical_address alloc_block();
void dealloc_block(physical_address add);
u32int memUsage();

#endif

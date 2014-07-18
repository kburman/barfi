#include <core/arch_x86/typedef.h>
#include <core/arch_x86/regs.h>
#include <core/arch_x86/irq.h>
#include <core/kmalloc.h>
#include <core/paging.h>
#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

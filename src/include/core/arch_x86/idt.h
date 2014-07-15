#ifndef __IDT_H__
#define __IDT_H_
#include <core/arch_x86/typedef.h>

void idt_set_gate(unsigned char num, unsigned long base, unsigned short sel, unsigned char flags);
void idt_install();

#endif

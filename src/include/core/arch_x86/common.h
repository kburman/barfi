#ifndef __COMMON_H__
#define __COMMON_H__
#include <core/arch_x86/typedef.h>

void outb(u16int port, u8int value);
u8int inb(u16int port);

void memcpy(u8int *dest, const u8int *src, u32int len);
void memset(u8int *dest, u8int val, u32int len);
unsigned short *memsetw(unsigned short *dest, unsigned short val, int count);

#endif

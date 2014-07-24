#ifndef __COMMON_H__
#define __COMMON_H__
#include <core/arch_x86/typedef.h>
 
// write a byte to port
void outb(u16int port, u8int value);

// read a byte from the port
u8int inb(u16int port);

// copy the [len] bytes from src to dest
void memcpy(u8int *dest, const u8int *src, u32int len);

// set [len] byte in dest as [val]
void memset(u8int *dest, u8int val, u32int len);

// set two byte at [dest] to [val] upto [count]
u16int *memsetw(u16int *dest, u16int val, u32int count);

#endif

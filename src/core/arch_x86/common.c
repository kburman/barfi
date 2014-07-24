#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>

// write a byte to port
void outb(u16int port, u8int value)
{
    __asm__ __volatile__ ("outb %1, %0" : : "dN" (port), "a" (value));
}

// read a byte from the port
u8int inb(u16int port)
{
    u8int ret;
    __asm__ __volatile__("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

// copy the [len] bytes from src to dest
void memcpy(u8int *dest, const u8int *src, u32int len)
{
    const u8int *sp = (const u8int *)src;
    u8int *dp = (u8int *)dest;
    for(; len != 0; len--) *dp++ = *sp++;
}

// set [len] byte in dest as [val]
void memset(u8int *dest, u8int val, u32int len)
{
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

// set two byte at [dest] to [val] upto [count]
u16int *memsetw(u16int *dest, u16int val, u32int count)
{
    unsigned short *temp = (unsigned short *)dest;
    for( ; count != 0; count--) *temp++ = val;
    return dest;
}

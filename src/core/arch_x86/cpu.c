#include <core/arch_x86/typedef.h>
#include <core/arch_x86/cpu.h>

u8int readCMOS(u8int addr)
{
   u8int ret;
   outb(0x70,addr);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   ret = inb(0x71);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   return ret;
}

void writeCMOS(u8int addr, u16int value)
{
   outb(0x70, addr);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
   outb(0x71, value);
   __asm__ __volatile__ ("jmp 1f; 1: jmp 1f;1:");
}

void reboot()
{
    outb (0x64, 0xFE); /* send reboot command */
}


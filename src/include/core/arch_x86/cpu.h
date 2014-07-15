#ifndef __CPU_H__
#define __CPU_H__
#include <core/arch_x86/typedef.h>

u8int readCMOS(u8int addr);
void writeCMOS(u8int addr, u16int value);
void reboot();






#endif
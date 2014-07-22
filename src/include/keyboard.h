#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__
#include <core/arch_x86/typedef.h>
#include <core/arch_x86/regs.h>

void keyboard_install();
void keyboard_handler(struct regs *r);
u8int getch();
void gets(u8int *ptr);
void hookforkeystroke(void (*func)(u8int key));


#endif

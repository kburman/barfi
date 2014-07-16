#ifndef __KEYBOARD_H__
#define __KEYBOARD_H__

void keyboard_install();
void keyboard_handler(struct regs *r);
u8int getch();
void gets(u8int *ptr);

#endif

#ifndef __IDT_H__
#define __IDT_H_

void isrs_install();
void fault_handler(struct regs *r);

#endif

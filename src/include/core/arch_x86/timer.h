#ifndef __TIMER_H__
#define __TIMER_H__
#include <core/arch_x86/typedef.h>
#include <core/arch_x86/regs.h>
void timer_phase(u32int hz);

// register for timer event at [msec] milli second
void register_tiemr_event(void (*func)(),u8int msec);

// it will be called when an timer irq
// is raised 
void timer_handler(struct regs *r);

// start timer
void timer_install();

// wait for xticks
void timer_wait(int xticks);

#endif

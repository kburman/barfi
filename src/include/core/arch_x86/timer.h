#ifndef __TIMER_H__
#define __TIMER_H__
#include <core/arch_x86/typedef.h>
#include <datetime.h>
void timer_phase(u16int hz);
void timer_install();
void timer_wait(int ticks);
void timer_handler(struct regs *r);
datetime_t getDatetime();
void timereventhandler(void (*f)(),u8int msec);
#endif

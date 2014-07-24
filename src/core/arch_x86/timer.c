#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/arch_x86/regs.h>
#include <core/arch_x86/irq.h>


// ticks 1000 times per second then 
// it will overflow in
// 584942417.355072032 year
volatile unsigned long timer_ticks = 0;
// function pointer to external program
// which will called to do any task which
// they want
void (*handler_func)();
// count the milli sec pass for handler function
u8int handler_counter;
// contain the requested milli second 
u8int handler_msec_passed;

void timer_phase(u32int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}

// register for timer event at [msec] milli second
void register_tiemr_event(void (*func)(),u8int msec)
{
    // if the f !=0 then move further
    if (func != 0)
    {
      handler_func = func;
      handler_counter = msec;
      handler_msec_passed = 0;
    }
}

// it will be called when an timer irq
// is raised 
void timer_handler(struct regs *r)
{
    timer_ticks++;
    if (handler_func != 0 && (handler_msec_passed++ >= handler_counter))  
    {
        handler_func();
    }

    //if(ticks%1000 == 0) puts("one second\n");
}

// start timer
void timer_install()
{
    handler_func = 0;
    handler_msec_passed = 0;
    handler_counter = 0;
    irq_install_handler(0, timer_handler);
    timer_phase(1000); // set to ticks 1000times per second

}

// wait for xticks
void timer_wait(int xticks)
{
    unsigned long eticks;
    eticks = timer_ticks + xticks;
    for(;eticks>timer_ticks;);
}






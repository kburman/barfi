#include <core/arch_x86/typedef.h>
#include <core/arch_x86/cpu.h>
#include <core/arch_x86/common.h>
#include <core/arch_x86/regs.h>
#include <core/arch_x86/irq.h>
#include <core/string.h>
#include <core/console.h>
#include <datetime.h>

#define BCD2BIN(bcd) ((((bcd)&15) + ((bcd)>>4)*10))
#define MINUTE 60
#define HOUR (60*MINUTE)
#define DAY (24*HOUR)
#define YEAR (365*DAY)

// ticks every 0.01 sec by default
volatile unsigned long ticks = 0;

void timer_phase(int hz)
{
    int divisor = 1193180 / hz;       /* Calculate our divisor */
    outb(0x43, 0x36);             /* Set our command byte 0x36 */
    outb(0x40, divisor & 0xFF);   /* Set low byte of divisor */
    outb(0x40, divisor >> 8);     /* Set high byte of divisor */
}
	

void timer_handler(struct regs *r)
{
    ticks++;
    //if(ticks%100 == 0) puts("one second\n");
}

void timer_install()
{
    irq_install_handler(0, timer_handler);
    timer_phase(100);
}

void timer_wait(int xticks)
{
    unsigned long eticks;
    char name[2];
    eticks = ticks + xticks;
    for(;eticks>ticks;);
}

//Gets CMOS actual time
datetime_t getDatetime()
{
   datetime_t now;

    __asm__ __volatile__ ("cli");
   now.sec = BCD2BIN(readCMOS(0x0));
   now.min = BCD2BIN(readCMOS(0x2));
   now.hour = BCD2BIN(readCMOS(0x4));
   now.day = BCD2BIN(readCMOS(0x7));
   now.month = BCD2BIN(readCMOS(0x8));
   now.year = BCD2BIN(readCMOS(0x9));
   __asm__ __volatile__ ("sti");

   return now;
}




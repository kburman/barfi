#include <core/arch_x86/typedef.h>
#include <datetime.h>
#define BCD2BIN(bcd) ((((bcd)&15) + ((bcd)>>4)*10))
#define MINUTE 60
#define HOUR (60*MINUTE)
#define DAY (24*HOUR)
#define YEAR (365*DAY)

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
#ifndef __DATETIME_H__
#define __DATETIME_H__

typedef struct{
   unsigned char sec;
   unsigned char min;
   unsigned char hour;
   unsigned char day;
   unsigned char month;
   unsigned char year;
}datetime_t;

//Gets CMOS actual time
datetime_t getDatetime();

#endif

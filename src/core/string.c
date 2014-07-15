#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <datetime.h>

u8int strlen(u8int *ptr)
{
	u8int len = 0;
	while(len != '\0')
	{
		len++;
	}
	return len;
}

void parseint(u16int no,u8int *dest)
{
	// reverse the nummber
	u16int rev =0;
	
	while(no != 0)
	{
		rev = rev*10 + no%10;
		no = no/10;
	}
	if(rev == 0) *dest++ = '0';
	while(rev != 0)
	{
		*dest++ = '0' + rev%10;
		rev = rev/10;
	}
	*dest = '\0';
}

void strcat(u8int *dest,u8int *src)
{
	while(*dest++ != '\0');
	dest--;
	while(*src != '\0') 
		*dest++ = *src++;
	*dest = '\0';
}

void parsedatetime(datetime_t *dt,u8int *ptr)
{
	*ptr = '\0';
	char tmp[5] = "";
	
	itoa(tmp,10,dt->year);
	strcat(tmp,ptr);
	strcat("/",ptr);
	
	itoa(tmp,10,dt->month);
	strcat(tmp,ptr);
	strcat("/",ptr);
	
	itoa(tmp,10,dt->day);
	strcat(tmp,ptr);
	strcat(" ",ptr);
	
	itoa(tmp,10,dt->hour);
	strcat(tmp,ptr);
	strcat(":",ptr);
	
	itoa(tmp,10,dt->min);
	strcat(tmp,ptr);
	strcat(":",ptr);
	
	itoa(tmp,10,dt->sec);
	strcat(tmp,ptr);
	strcat(":",ptr);
}

/* Taken from Linux */
void itoa (char *buf, int base, int d)
{
       char *p = buf;
       char *p1, *p2;
       unsigned long ud = d;
       int divisor = 10;
     
       /* If %d is specified and D is minus, put `-' in the head. */
       if (base == 'd' && d < 0)
         {
           *p++ = '-';
           buf++;
           ud = -d;
         }
       else if (base == 'x')
         divisor = 16;
     
       /* Divide UD by DIVISOR until UD == 0. */
       do
         {
           int remainder = ud % divisor;
     
           *p++ = (remainder < 10) ? remainder + '0' : remainder + 'a' - 10;
         }
       while (ud /= divisor);
     
       /* Terminate BUF. */
       *p = 0;
     
       /* Reverse BUF. */
       p1 = buf;
       p2 = p - 1;
       while (p1 < p2)
        {
           char tmp = *p1;
           *p1 = *p2;
           *p2 = tmp;
           p1++;
           p2--;
       }
}



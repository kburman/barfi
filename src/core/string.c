#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>

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

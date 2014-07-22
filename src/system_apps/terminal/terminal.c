#include <system_apps/terminal/terminal.h>
#include <core/arch_x86/typedef.h>
#include <core/arch_x86/multiboot.h>
#include <core/arch_x86/timer.h>
#include <core/arch_x86/kmalloc.h>
#include <core/console.h>
#include <keyboard.h>
#include <datetime.h>
#include <core/string.h>
#define USERINPUT_LIMIT 30

// forward declaration
void keyhandler(u8int key);
u8int* matchcmd(u8int* cmd,u8int* userinput);
extern void timereventhandler(void (*f)(),u8int);
void timer_event();
extern multiboot_header_t *mbd_ptr;
extern u32int stack_start; // stack grows downward
extern u32int *end;
extern u32int *code;
extern u32int frame_count; // memory manged by kmalloc
extern u32int blocks_allocated_kernel;
// variable
unsigned char keyboard[] =
{
    0,  27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't','y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';','\'', '`', 0, '\\', 'z',
    'x', 'c', 'v', 'b', 'n','m', ',', '.', '/', 0,'*', 0,' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 254, 0, '-', 252, 0, 253, '+', 0, 255, 0, 0, 0, 0, 0, 0, 0,
    0, 0,
};
u8int indx;
u8int buffer[USERINPUT_LIMIT];


void start_terminal()
{
    setcolor(0x98);
    memset(buffer,0,USERINPUT_LIMIT);
    clrscr();
    indx = 0;
    hookforkeystroke(keyhandler);
    //timereventhandler(timer_event,10);
    printCommandLine();
}

void printCommandLine()
{
    printHeader();
    setxy(0,1);
    printprompt();
}

void clearCommandLine()
{
    int i;
    setxy(0,1);
    setcolor(0x98);
    for(i=0; i<80; i++) putchar(' ');
    indx = 0;
}

void printprompt()
{
    setcolor(0x98);
    puts(">> ");
    indx = 0;
}

void clearStage()
{
	int i;
	setxy(0,2);
	for(i=0;i<80*23;i++) putchar(' ');
}

void keyhandler(u8int key)
{
    setcolor(0x9f);
    if(indx >= 0 && indx < USERINPUT_LIMIT)
    {
        if(keyboard[key] == '\b')
        {
            if(indx > 0)
            {
                putchar('\b');
                indx--;
            }
        }
        else if(keyboard[key] == '\n')
        {
            buffer[indx++] = '\0';
            clearStage();
            processCommand();
            clearCommandLine();
            printCommandLine();
        }
        else if(indx < USERINPUT_LIMIT-1) // one minus because we have to add null char and so that bckspace can wrk
        {
            putchar(keyboard[key]);
            buffer[indx] = keyboard[key];
            indx++;
        }
    }
}

void processCommand()
{
	u8int *ptr;
    setxy(0,2);
    setcolor(0x9f);

    if((ptr = matchcmd("echo",buffer)) != 0)
    {
    	puts(ptr);
    }
    else if((ptr = matchcmd("now",buffer)) != 0)
    {
    	datetime_t now;
    	now = getDatetime();
    	memset(buffer,0,USERINPUT_LIMIT);
    	parsedatetime(&now,buffer);
    	puts(buffer);
    }
    else if((ptr = matchcmd("test",buffer)) != 0)
    {
    	char *ptr = "hello";
    	char ptr2[10] = "world";
    	strcat(ptr,ptr2);
    	puts(ptr2);
    }
    else if((ptr = matchcmd("reboot",buffer)) != 0)
    {
    	reboot();
    }
    else if((ptr = matchcmd("mem",buffer)) != 0)
    {
		setcolor(0x9f);
		puts("Memory manged : ");setcolor(0x9e);putint(frame_count*8*4);puts(" KB\n");

		setcolor(0x9f);
		u32int memu = memUsage()*4;
		puts("Total Memory Used : ");setcolor(0x9e);putint(memu);puts(" KB\n");

		setcolor(0x9f);
		memu -= 0x00100000/1024;
		puts("Total Memory Used(after grub) : ");setcolor(0x9e);putint(memu);puts(" KB\n");

		setcolor(0x9f);
		puts("Memory Used by kernel: ");setcolor(0x9e);putint(blocks_allocated_kernel*4 - 0x00100000/1024);puts(" KB\n");
    }
    else if((ptr = matchcmd("boot",buffer)) != 0)
    {
		setcolor(0x9f);
		puts("Ram  Size : ");setcolor(0x90);putint((mbd_ptr->mem_upper));putchar('\n');
		setcolor(0x9f);
		puts("Command Line : ");setcolor(0x90);puts(mbd_ptr->cmdline);putchar('\n');
		setcolor(0x9f);
		puts("Bootloader name : ");setcolor(0x90);puts(mbd_ptr->boot_loader_name);putchar('\n');
    }
    else if((ptr = matchcmd("help",buffer)) != 0)
    {
    	setcolor(0x9f);
    	puts("help - this page\n");
    	puts("echo - print any thing you write\n");
    	puts("now  - print the date and time\n");
    	puts("boot  - display some info from multiboot header\n");
    	puts("mem  - display some info from multiboot header\n");
    }
    else
    {
    	setcolor(0x94);puts(buffer);
    	setcolor(0x9a);puts(" is not any recognied command.\nTry typing ");
    	setcolor(0x9a);puts("help");
    }
}


// return 0 if not matched
// or return pointer to next arg
u8int* matchcmd(u8int* cmd,u8int* userinput)
{
	while (*cmd == *userinput)
	{
		if(*cmd == '\0') break;
		cmd++;
		userinput++;
	}
	if( (*cmd == '\0') && (*userinput == 0 || *userinput == ' ') )
		return userinput;
	else
		return 0;
}

void timer_event()
{
    printHeader();
}

void printHeader()
{
    int i,tmpx,tmpy;
    getxy(&tmpx,&tmpy);
    setxy(0,0);
    setcolor(0x87);
    for(i=0; i<37; i++) putchar(' ');
    puts("barfi");
    for(i=0; i<38; i++) putchar(' ');
    setxy(tmpx,tmpy);
    setcolor(0x98);
}

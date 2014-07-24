#include <system_apps/terminal/terminal.h>
#include <core/arch_x86/typedef.h>
#include <core/arch_x86/multiboot.h>
#include <core/arch_x86/kmalloc.h>
#include <core/arch_x86/common.h>
#include <core/console.h>
#include <keyboard.h>
#include <datetime.h>
#include <core/string.h>
#define USERINPUT_LIMIT 30u
#define DEFAULT_COLOR 0x98
#define COMMOND_COLOR 0x9f
#define DEFAULT_PROMT ">>"
#define PRINT_HEADER 1
#ifdef PRINT_HEADER
    #define CMD_X 0
    #define CMD_Y 1
#else
    #define CMD_X 0
    #define CMD_Y 0
#endif

// forward declaration
extern multiboot_header_t *mbd_ptr;
extern u32int frame_count;
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
s8int buffer[USERINPUT_LIMIT];

//  start the terminal application
// anything on the screen will be
// erased
void start_terminal()
{
    // set the console color to the default
    setcolor(DEFAULT_COLOR);
    // clear the buffer memory where we will
    // store the text entered by the user
    memset((u8int*)buffer,0u,USERINPUT_LIMIT);
    // clear the screen with the deafult color
    clrscr();
    // set the index which represent the number
    // of charecter user typed
    indx = 0;
    // hook keyboard for key event
    hookforkeystroke(keyhandler);
    //timereventhandler(timer_event,10);
    // print ">> "
    printCommandLine();
}

// print the header and commond prompt [>>]
void printCommandLine()
{
    #ifdef PRINT_HEADER
        printHeader();
    #endif
    // move the cursor to the x=0,y=1
    setxy(CMD_X,CMD_Y);
    // now print the prompt
    printprompt();
}

// clear any text tpyed by the user in command area
void clearCommandLine()
{
    u8int i;
    setxy(CMD_X,CMD_Y);
    setcolor(DEFAULT_COLOR);
    // clear the command line by writing
    // space there 80 times which will fill the
    // whole line.
    for(i=0; i<80; i++) putchar(' ');
    // reset the index
    indx = 0;
}

// put the promt in the screen
void printprompt()
{
    setcolor(DEFAULT_COLOR);
    puts(DEFAULT_PROMT);
    indx = 0;
}

// clear the area where result is shown
void clearStage()
{
	int i;
    // just move to next line after the cmd prompt
	setxy(CMD_X,CMD_Y+1);
	for(i=0;i<80*(24-CMD_Y);i++) putchar(' ');
}

// this will process the key event for us
// and decode if to print it on screen or not
void keyhandler(u8int key)
{
    setcolor(COMMOND_COLOR);
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
            // here we can add additional check up to
            // ensure that if char is what we expected
            putchar(keyboard[key]);
            buffer[indx] = keyboard[key];
            indx++;
        }
    }
}

// parse the command typed by the user
// and produce outpu for it
void processCommand()
{
	s8int *ptr;
    setxy(CMD_X,CMD_Y+1);
    setcolor(DEFAULT_COLOR);

    if((ptr = matchcmd("echo",buffer)) != 0)
    {
    	puts(ptr);
    }
    else if((ptr = matchcmd("now",buffer)) != 0)
    {
    	datetime_t now;
    	now = getDatetime();
    	memset((u8int*)buffer,0u,USERINPUT_LIMIT);
    	parsedatetime(&now,buffer);
    	puts(buffer);
    }
    else if((ptr = matchcmd("test",buffer)) != 0)
    {
    	memset((u8int*)buffer,0u,USERINPUT_LIMIT);
        itoa(buffer,'x',15);
        puts(buffer);
    }
    else if((ptr = matchcmd("reboot",buffer)) != 0)
    {
    	reboot();
    }
    else if((ptr = matchcmd("mem",buffer)) != 0)
    {
		setcolor(0x9f);
		puts("Memory manged : ");setcolor(0x9e);putint(frame_count*4);puts(" KB\n");

		setcolor(0x9f);
		u32int memu = getMemoryConsumed_kb();
		puts("Total Memory Used : ");setcolor(0x9e);putint(memu);puts(" KB\n");

		setcolor(0x9f);
		memu -= 0x00100000/1024;
		puts("Total Memory Used(after grub) : ");setcolor(0x9e);putint(memu);puts(" KB\n");

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
    	setcolor(DEFAULT_COLOR);
    	puts("help - this page\n");
    	puts("echo - print any thing you write\n");
    	puts("now  - print the date and time\n");
    	puts("boot  - display some info from multiboot header\n");
    	puts("mem  - display some info from multiboot header\n");
    }
    else
    {
    	setcolor(0x94);puts(buffer);
    	setcolor(DEFAULT_COLOR);puts(" is not any recognied command.\nTry typing ");
    	setcolor(0x9a);puts("help");
    }
}


// return 0 if not matched
// or return pointer to next arg
s8int* matchcmd(s8int* cmd,s8int* userinput)
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

// print the header
void printHeader()
{
    // as preventave measure to  not print
    // header if any one call it.
    #ifdef PRINT_HEADER
        int i,tmpx,tmpy;
        getxy(&tmpx,&tmpy);
        setxy(0,0);
        setcolor(0x87);
        for(i=0; i<37; i++) putchar(' ');
        puts("barfi");
        for(i=0; i<38; i++) putchar(' ');
        setxy(tmpx,tmpy);
        setcolor(DEFAULT_COLOR);
    #else
        // this is really important it may be the case
        // if we forgot about #ifdef and called printHeader
        // and found nothing is being printed
        clrscr();
        puts("You have called printHeader() but ");
        puts("it PRINTI_HEADER is to 0.");
        for(;;);
    #endif
}

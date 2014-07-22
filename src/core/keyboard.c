#include <core/arch_x86/typedef.h>
#include <core/arch_x86/regs.h>
#include <core/arch_x86/irq.h>
#include <core/console.h>
#include <core/arch_x86/common.h>
#include <keyboard.h>

volatile u8int shift_flag = 0;
void (*handle_keystroke)(u8int scancode);	// keystroke handler

void hookforkeystroke(void (*func)(u8int key))
{
	if(func) handle_keystroke = func;
}

void keyboard_install()
{
    handle_keystroke = 0;
    irq_install_handler(1, keyboard_handler);
}

void keyboard_handler(struct regs *r)
{
    unsigned char scancode;
    scancode = inb(0x60);
    switch(scancode)
    {
        case 0x3a: // CAPS LOCK LED
            break;
        case 0x45: // NUM LOCK LED
            break;
        case 0x46: // SCROLL LOCK
            break;
        case 59:
            break;
        case 60: // f2
            reboot();
            break; // this is useless
    }

    if(scancode & 0x80) // key is released
    {
        switch(scancode-0x80)
        {
            case 42:
            case 54:
                shift_flag = 0;
                break;
        }
    }
    else
    {
        switch(scancode)
        {
            case 42:
            case 54:
                shift_flag = 1;
                break;
        }
        // CALL SOME OTHER FUNC TO HANDLE IT
        if(handle_keystroke)
        {
        	handle_keystroke(scancode);
        }
        //putint(scancode);
    }

}





















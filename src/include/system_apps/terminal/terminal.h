#ifndef __TERMINAL_H__
#define __TERMINAL_H__
#include <core/arch_x86/typedef.h>

//  start the terminal application 
// anything on the screen will be 
// erased 
void start_terminal();

// print the header and commond prompt [>>]
void printCommandLine();

// clear any text tpyed by the user in command area
void clearCommandLine();

// put the promt in the screen
void printprompt();

// clear the area where result is shown
void clearStage();

// this will process the key event for us 
// and decode if to print it on screen or not
void keyhandler(u8int key);

// parse the command typed by the user
// and produce outpu for it
void processCommand();

// return 0 if not matched
// or return pointer to next arg
s8int* matchcmd(s8int* cmd,s8int* userinput);

// print the header
void printHeader();

#endif

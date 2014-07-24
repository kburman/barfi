#ifndef __KMALLOC_H__
#define __KMALLOC_H__
#include <core/arch_x86/typedef.h>

// this func calculate the number of frames
// require for the ram and then acc to that
// create no. of frames which will be required for it
// and also mark the space which is preoccupied
// by the kernel
void init_mm(u32int ram_sz_bytes); 

// this will allocate space for frames
// and fill it with zero removing any
// garbage value
void initFrames(u32int ram_sz_bytes);

// mark the frame to be occupied in the 
// bitmap as the result it will not be
// in short it it reserved
// expect 4kb alinged address
// any other address will be 4kb 
// aligned with floor func
// means 10kb will became 8kb not 12kb
void setFrame(physical_address address);

// mark the address in the bitmap in the
// bitmap as free so any it can allocated
// in future.
// it expext that address is 4kb 
// any other address will be automatically 
// made 4kb alinged with floor function
// means 10kb will be made 8kb not 12kb
void removeFrame(physical_address address);

// it will find the first 4kb block which 
// is available for allocation and return 
// its physical address 4kb alinged
// return 0 if nothing found
physical_address findFirst();

// allocate the space for kernel
// by marking it in bitmap
// so that that space may not be allocated
// to any other purpose.
void allocate_kernel();

// it will find a free frame and set
// it as occupied and return the 
// physical address to that frame
// it will return 4kb alinged address
// return 0 if no free frame found
physical_address alloc_block();

// it will mark the frame that belong
// to that address as being unoccuppied
void dealloc_block(physical_address add);

// this include the kernel also
// since kernel code start at 1mb
// there will be min 1mb usage
// return in kb
u32int getMemoryConsumed_kb();
#endif

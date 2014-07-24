#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/arch_x86/kmalloc.h>
#define FRAME_SIZE 4096

// it tells us where thhe kernel ends
extern u32int end; 
// pointer to the first frame
u8int *frames;
// number of 4kb chunks 
u32int frame_count;
// count the number of frames actually got allocated
// after kernel and bitmap in 4kb chunks
u32int frame_allocated;


// this func calculate the number of frames
// require for the ram and then acc to that
// create no. of frames which will be required for it
// and also mark the space which is preoccupied
// by the kernel
void init_mm(u32int ram_sz_bytes)
{
	// create a bitmap for the record
	// which blocks is allocated
	// and which isn't
	initFrames(ram_sz_bytes);
	// right now bit is full of zero 
	// means nothing is allocated yet
	// so we have place one to show that 
	// kernel is here you can't allocate
	// here
	allocate_kernel();
}

// this will allocate space for frames
// and fill it with zero removing any
// garbage value
void initFrames(u32int ram_sz_bytes)
{
	// divide the ram size in 4kb blocks 
	frame_count = ram_sz_bytes / (FRAME_SIZE);
	// bitmap will be stored just after
	// the kernel ends
	frames = (u8int*)&end;
	// fill zero to remove garbage value
	// althrough some emulators fill zero's
	// as a kind gesture but real h/w does't
	// show such a kidness
	memset(frames,0,frame_count);
}

// mark the frame to be occupied in the 
// bitmap as the result it will not be
// in short it it reserved
// expect 4kb alinged address
// any other address will be 4kb 
// aligned with floor func
// means 10kb will became 8kb not 12kb
void setFrame(physical_address address)
{
	// we need to get the index in the frames
	// since each contain 8 4kb blocks
	// we need to divide address first
	// into 4kb blocks and then cluster 
	// them into 8 in short we have to 
	//  divide it by 8*4*1024
	u32int indx = (u32int)address / (8*FRAME_SIZE);
	// now it may be the case that address is not 
	// exactly divide by 8 there may some remainder
	// left so that will now act as offset
	u32int off = (u32int)address % (8*FRAME_SIZE);
	// curently variable off contain the byte left 
	// we need to make it in 4kb chunks so that we 
	// can get the which bit we need to accesss
	off /= FRAME_SIZE;
	// we assume that the supplied address is 4kb aligned 
	// because here we make it 4kb alinged any any more
	// precision will be lost here
    frames[indx] |= (0x1 << off);
    // increase the counter
    frame_allocated++;
}

// mark the address in the bitmap in the
// bitmap as free so any it can allocated
// in future.
// it expext that address is 4kb 
// any other address will be automatically 
// made 4kb alinged with floor function
// means 10kb will be made 8kb not 12kb
void removeFrame(physical_address address)
{
	// we need to get the index in the frames
	// since each contain 8 4kb blocks
	// we need to divide address first
	// into 4kb blocks and then cluster 
	// them into 8 in short we have to 
	//  divide it by 8*4*1024
	u32int indx = (u32int)address / (8*FRAME_SIZE);
	// now it may be the case that address is not 
	// exactly divide by 8 there may some remainder
	// left so that will now act as offset
	u32int off = (u32int)address % (8*FRAME_SIZE);
	// curently variable off contain the byte left 
	// we need to make it in 4kb chunks so that we 
	// can get the which bit we need to accesss
	off /= FRAME_SIZE;
	// we assume that the supplied address is 4kb aligned 
	// because here we make it 4kb alinged any any more
	// precision will be lost here
    frames[indx] &= ~(0x1 << off);
    // deduce the counter
    frame_allocated--;
}

// it will find the first 4kb block which 
// is available for allocation and return 
// its physical address 4kb alinged
// return 0 if nothing found
physical_address findFirst()
{
	// it will look up in the bitmap for any zero
	// it will tranlate the position of first zero
	// into physical address and return it.

	// TODO: check if u16 will be enough or not
	u16int i = 0;
	while(i < frame_count)
	{
		if (frames[i/8] == 0xff) // useless to serach here
		{
			i += 8;
		}
		else // here we have some thing free so look here
		{
			// we will check if the i%8 bit 
			// is zero or not
			if ( (frames[i/8] & (0x1 << i%8)) == 0x0 )
			{
				// we got a 4 kb chunk free now we
				// need to translate that address 
				// and return it
				return i*FRAME_SIZE;
			}
			i++;
		}
	}
	return 0;
}

// this include the kernel also
// since kernel code start at 1mb
// there will be min 1mb usage
// return in kb
u32int getMemoryConsumed_kb()
{
	return frame_allocated*4;
}

// allocate the space for kernel
// by marking it in bitmap
// so that that space may not be allocated
// to any other purpose.
void allocate_kernel()
{
	u32int tmp,i;
	// first of all we have to calculate
	// how much blocks we need for kernel
	// each block manage 4kb chuck
	//  1block = 4kb
	u32int block_count = ((u32int)&end)/FRAME_SIZE;
	// see if there is any space left as the
	// remainder in the above division if yes then 
	// we have to add a new block for it 
	tmp = ((u32int)&end)%FRAME_SIZE;
	if ( tmp != 0 )
	{
		// some bytes were left so add one more
		// 4kb chuck to acomodate it
		block_count++;
	}
	// now our bitmap also use some memory
	// after the kernel end so we should 
	// also count it 
	// one frame take 1bit
	// 8 frames = 1byte
	// since max 32bit can mange is 4gb 
	// bit map needd for 4gb = 128 bytes
	// 4096 bytes will be far enough for it
	block_count++;
	// now we need to set in the bitmap that 
	// all this blocks are already in use
	// so don't allocate it to any one
	memset(frames,0xff,block_count/8);
	// now suppose block_count was 9
	// dividing it by 8 we ignored one 4kb 
	// block which could be dangerous
	// so we have to mark it also
	tmp = block_count%8;
	if (tmp != 0)
	{
		// here in this for loop we will 
		// set 1 bit wise bit this may 
		// seems complex but it is to
		// simple try to read it with 
		// open mind.
		for (i=0;i<tmp;++i)
		{
			frames[block_count/8] |= (0x1 << tmp );
		}
	}	
	frame_allocated = block_count;
}

// it will find a free frame and set
// it as occupied and return the 
// physical address to that frame
// it will return 4kb alinged address
// return 0 if no free frame found
physical_address alloc_block()
{
	// first we will search for any free frame
	// is available or not.
	physical_address add = findFirst();
	// check if return address is not zero
	// which means we got some free frame
	if (add != 0)
	{
		// mark that frame which belong to that
		// address as occupied and then return 
		// that address.
		setFrame(add);
		return add;
	}
	return 0;
}

// it will mark the frame that belong
// to that address as being unoccuppied
void dealloc_block(physical_address add)
{
	removeFrame(add);
}
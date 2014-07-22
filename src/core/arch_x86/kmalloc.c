#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/arch_x86/kmalloc.h>


extern u32int end; // located in linker
u8int *frames; // point to our frames
u32int frame_count;
u32int ram_sz;	// in bytes
u32int blocks_allocated_kernel = 0;
u32int kernel_end; // denote the end of the kernel
void init_mm(u32int ram_sz_bytes)
{
	ram_sz = ram_sz_bytes;
	createFrames();
	allocate_kernel();
}

void createFrames()
{
	frame_count = ram_sz / (4*1024);
	frame_count /= 8;
	frames = (u8int*)&end;
	memset(frames,0,frame_count);
}

void setFrame(physical_address address)
{
	u32int indx = (u32int)address / (8*4*1024);
	u32int off = (u32int)address % (8*4*1024);
	off /= 4*1024;
    frames[indx] |= (0x1 << off);
}

void removeFrame(physical_address address)
{
	u32int indx = (u32int)address / (8*4*1024);
	u32int off = (u32int)address % (8*4*1024);
	off /= 4*1024;
    frames[indx] &= ~(0x1 << off);
}

physical_address findFirst()
{
	u32int i,j;
	for(i=0;i<frame_count;i++)
	{
		if(frames[i] != 0xff)
		{
			for(j=0;j<8;j++)
			{
				if(!(frames[i] & (0x1 << j)))
				return (physical_address)(i*8*4*1024 + j*4*1024);
			}
		}
	}
	return 0;
}

u32int memUsage()
{
    u32int i,j;
    u32int count = 0;
    for(i=0;i<frame_count;i++)
    {
		if(frames[i] == 0xff)
		{
			count += 8;
		}
		else
		{
			for(j=0;j<8;j++)
			{
				if((frames[i] & (0x1 << j))) count++;
			}
		}
    }
	return count;
}

void allocate_kernel()
{
	// calc blocks needed
	u32int block_count = 0;
	physical_address add = &end;
	add += frame_count;

	block_count = add/(4*1024);
	if((add%(4*1024)) != 0)
		block_count++;
    // for now we just fill the next 8block that is bad
    // but it is just for now
	if(block_count%8 != 0)
		block_count += 8 - block_count%8;
    memset(frames,0xff,block_count/8);
    blocks_allocated_kernel = block_count;
}

// allocate a 4kb block and return its physical address
physical_address alloc_block()
{
	physical_address add;
	add = findFirst();
	setFrame(add);
	return add;
}

void dealloc_block(physical_address add)
{
	removeFrame(add);
}





























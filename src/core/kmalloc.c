#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/kmalloc.h>
#define RAM_SIZE_KB 1024*16
/*
	How It Works
	First it will find a end of kernel and just after
	it it will allocate some space for maping space will
	be calculated by dividing ram_size in kb by 8
	why 8 because minimum amt of mem which can be allocated is 1kb
	and we will represent this block by 1 bit not 1 byte
	hence in place of one char (8 bit ) we can manage 8kb
	some calculation according to this method if we have to map
	4GB of ram then 4GB = 4194304KB
	since 1kb be repesented by 1 bit
	so ,4194304/8 =524288byte
	   ,524288byte = 512KB
	so we can map a ram of 4gb with only 512kb space

	block size = 1kb ; ram =4gb ; map size = 512kb
	block size = 1kb ; ram =2gb ; map size = 256kb

	block size = 2kb ; ram =4gb ; map size = 128kb
	block size = 2kb ; ram =2gb ; map size = 64kb
*/

u32int getKernelSize_a();
extern u32int end;
u8int *frames;	// points to the frames 0,1
u32int frames_count;	// in bytes

u32int getRamSizeKB()
{
	return (u32int)RAM_SIZE_KB;
}

// set the bit to one
void setFrame(u32int address)
{
	u16int indx = address/8;
	u8int off = address%8;
	frames[indx] |= (0x1 << off);
}

void setFrame(u32int address,u32int sz)
{
	while(sz != 0)
	{
		setFrame(address++);
		sz--;
	}
}

void removeFrame(u32int address)
{
	u16int indx = address/8;
	u8int off = address%8;
	frames[indx] &= ~(0x1 << off);
}


int getStatus(u32int address)
{
	u16int indx = address/8;
	u8int off = address%8;
	return (frames[indx] &(0x1 << off));
}

// if return zero then nothing is free
// sz in KB
u32int getFreeFrame(u32int sz)
{
	int i,j,z=0;
	for(i=0;i<frames_count;i++)
	{
		if(frames[i] != (u8int)0xff) // some block is free
		{
			//puts("pass\n");
			for(j=0;j<8;j++)
			{
				if((frames[i] & (0x1 << j)) == 0)
					z++;
				else
					z = 0;
				if(z == sz) return (8*i + j) - (sz-1);
			}
		}
	}
	return 0;
}

void inti_mm()
{
	createFrameMap();
	//allocate_kernel();
	testAllocation();
	if(getStatus(getKernelSize_a()-1) != 0)
	{
		puts("Working");
	}
	putint(getKernelSize_a());
	printDebugInfo();
}

u32int kmalloc(u32int sz)
{
	u32int add = getFreeFrame(sz);
	if(add != 0)
	{
		setFrame(add,sz);
	}
}

void testAllocation()
{
    setFrame(0);
    setFrame(1);

    setFrame(3);
    setFrame(4);
    setFrame(5);
    setFrame(6);



    setFrame(10);
    setFrame(11);


    u32int add = kmalloc(1);
	add = kmalloc(1);
    puts("\nRequest for 4kb gives us : ");
    putint((u32int)add);
    putchar('\n');
}

// return the kernel size 1kb alinged
// in kb
u32int getKernelSize_a()
{
	u32int kernel_size = (u32int)&end;
	kernel_size += frames_count;
	if(kernel_size%1024 == 0) // means  1kb alinged
	{
		kernel_size /= 1024;
	}
	else
	{
		kernel_size /= 1024;
		kernel_size++; // add one kb
	}
	return kernel_size;
}

void allocate_kernel()
{
	u32int kernel_size = getKernelSize_a();
	int iKB = 0;
	for(iKB = 0;iKB < kernel_size;iKB++)
	{
		setFrame(iKB,1);
	}
}

void createFrameMap()
{
	frames = &end;
	frames_count = getRamSizeKB() / 8;
	memset(frames,0,frames_count);
}


void printDebugInfo()
{
	puts("\n\n------DEBUG INFO : kmalloc----------\n\n");
	puts("end : ");
	putint((u32int)&end);
	puts("\nno. of frames(8bit each) :");
	putint(getRamSizeKB()/8);
	puts("\nFirst free frame is at : ");
	putint((int)getFreeFrame(1));
	puts("\n\n---------------------------------------\n\n");
}

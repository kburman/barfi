#include <core/arch_x86/typedef.h>
#include <core/arch_x86/common.h>
#include <core/kmalloc.h>
#define debug_frames_func1
#define debug_allocation

typedef u32int physical_address;
extern u32int end; // located in linker
u8int *frames; // point to our frames
u32int frame_count;
u32int ram_sz;	// in bytes

void init_mm(u32int ram_sz_bytes)
{
	ram_sz = ram_sz_bytes;
	createFrames();
	#ifdef debug_frames_func
		testFrame();
	#endif // debug
	allocate_kernel();
	#ifdef debug_allocation
		testAllocation();
	#endif // debug
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
	#ifdef debug_frames_func
		setcolor(0x02);
		puts("\nfrom setframe phy:");putint(address);puts(" i:");putint(indx);puts(" o:");putint(off);putchar('\n');
	#endif // debug
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
                {
					#ifdef debug_frames_func
						setcolor(0x30);
						puts("\nfrom findf phy:");putint((physical_address)(i*8*4*1024 + j*4*1024));puts(" i:");putint(i);puts(" j:");putint(j);putchar('\n');
					#endif // debug
					return (physical_address)(i*8*4*1024 + j*4*1024);
                }
			}
        }
	}
	return 123;
}

// this test should be run before alloting any thing to frames;
//
#ifdef debug_frames_func
void testFrame()
{
	//puts("\n--------- DEBUG kmalloc-> frames --------------\n");
	puts("Test 1   : ");
	physical_address add;
	setFrame(1);
	add = findFirst();
	if(add == 0)
		puts("Pass");
	else
		puts("Fail");

	puts("\nTest 2   : ");
	removeFrame(1);
	setFrame(0);
	add = findFirst();
	if(add == 1)
		puts("Pass");
	else
		puts("Fail");
	removeFrame(0);
	//puts("\n-------------------------------------------\n");
}
#endif


#ifdef debug_allocation

// run this after kernel allocation
void testAllocation()
{
	setcolor(0x09);
	puts("\n--------- DEBUG kmalloc-> frames ---------------------------\n");
	physical_address add;
	physical_address first;

	add = findFirst();
	first = add;
	setFrame(add);
	setcolor(0x0f);	puts("\naloc0 at : ");setcolor(0x03); putint(add);putchar('\n');

	add = findFirst();
	setFrame(add);
	setcolor(0x0f);	puts("aloc1 at : ");setcolor(0x03); putint(add);putchar('\n');

	add = findFirst();
	setFrame(add);
	setcolor(0x0f);	puts("aloc2 at : ");setcolor(0x03); putint(add);putchar('\n');

	first = add;
	add = findFirst();
	setFrame(add);
	setcolor(0x0f);	puts("aloc3 at : ");setcolor(0x03); putint(add);putchar('\n');

	add = findFirst();
	setFrame(add);
	setcolor(0x0f);	puts("aloc4 at : ");setcolor(0x03); putint(add);putchar('\n');

	removeFrame(first);
	setcolor(0x0f);	puts("dealoc2 at : ");setcolor(0x03); putint(first);putchar('\n');

	// 4
	add = findFirst();
	setFrame(add);
	setcolor(0x0f);	puts("aloc5 at : ");setcolor(0x03); putint(add);putchar('\n');

	setcolor(0x09);
	puts("\n------------------------------------------------------------\n");
}

#endif

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

    #ifdef debug
		puts("\n--------- DEBUG kmalloc-> frames --------------\n");
		puts("Kernel total address : ");
		putint(add);
		puts("\n");

		puts("Kernel allocated Block count : ");
		putint(block_count);
		puts("\n-----------------------------------------------\n");
	#endif
}





























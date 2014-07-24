
global enablePaging
extern kernel_dir
enablePaging:
	mov eax,[kernel_dir]
	mov cr3,eax
	
	mov eax,cr0
	or eax,0x80000000
	mov cr0,eax
	ret

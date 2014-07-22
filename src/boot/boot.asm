; Bootloader

;These are the multiboot macroes, they are used to configure how GRUB
;loads the kernel

MBOOT_PAGE_ALIGN    equ 1<<0 ;Loads everything on page boundaries
MBOOT_MEM_INFO      equ 1<<1 ;Load memory map
MBOOT_HEADER_MAGIC  equ 0x1BADB002 ; Required (MAGIC)

MBOOT_HEADER_FLAGS  equ MBOOT_PAGE_ALIGN | MBOOT_MEM_INFO
MBOOT_CHECKSUM      equ -(MBOOT_HEADER_MAGIC + MBOOT_HEADER_FLAGS)


[BITS 32]

[GLOBAL mboot]
[EXTERN code] ;.text
[EXTERN bss]  ;.bss
[EXTERN end]  ;defined in linker script

mboot:
  dd  MBOOT_HEADER_MAGIC
  dd  MBOOT_HEADER_FLAGS
  dd  MBOOT_CHECKSUM

  dd  mboot
  dd  code
  dd  bss
  dd  end
  dd  start ;Initial EIP

[GLOBAL start] ;Entry Point
[EXTERN kernel_start]

start:
  mov esp, _sys_stack     ; This points the stack to our new stack area
  push    esp ;Stack location (GRUB doesn't tell us)
  push    ebx ;Multiboot header location

  ; Execute the kernel:
  cli
  call kernel_start
  hlt


SECTION .bss
    resb 8192               ; This reserves 8KBytes of memory here
_sys_stack:
	


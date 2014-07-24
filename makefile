# DEBUG
DEBUG = 

# COMPILER 
CC  = gcc
CF  = -Wall -O  -fstrength-reduce -fomit-frame-pointer 
CF += -fno-stack-protector -finline-functions -nostdinc 
CF += -fno-builtin  -m32  -I./src/include -c

# LINKER
LNK = ld
LNKF = -Map map.txt -m elf_i386 -s 

#mkisofs
MK = mkisofs
MKF = -no-emul-boot -boot-load-size 4 -boot-info-table -N -J -r -c _$$ -hide-joliet _$$ -hide _$$ 

# C FILES
C_SOURCES = $(shell find src -type f -iname '*.c')
C_OBJECTS = $(foreach x,$(basename $(C_SOURCES)),$(x).o)

# ASM_FILES
ASM_SOURCES = $(shell find src -type f -iname '*.asm')
ASM_OBJECTS = $(foreach x,$(basename $(ASM_SOURCES)),$(x).o)

# Output files
KERNEL_BIN = bin/kernel.bin
ISO_FILE = barfi.iso
KERNEL_OBJDUMP = kernel_objdump.txt

all : cleanall compile link  makeiso runqemu

cleanall:
	#Cleaning all genrated files
	@rm -f $(C_OBJECTS) $(ASM_OBJECTS) $(KERNEL_BIN) $(KERNEL_OBJDUMP) map.txt

compile: $(C_OBJECTS) $(ASM_OBJECTS)

%.o:%.c
	@$(CC) $(CF) $(DEBUG) -o $@  $<
	
%.o:%.asm
	@nasm $(DEBUG) -felf $< -o $@

link: $(C_OBJECTS) $(ASM_OBJECTS)
	@$(LNK) $(DEBUG) $(LNKF) -Tlinker.ld -o $(KERNEL_BIN)  $(C_OBJECTS) $(ASM_OBJECTS)
	@objdump -M intel -x -d -s -S -t -g -e $(KERNEL_BIN) > kernel_objdump.txt

	
makeiso: $(KERNEL_BIN)
	$(MK) -o $(ISO_FILE) -b isolinux/isolinux.bin $(MKF) ./bin
	
runbochs: $(ISO_FILE)
	@bochs -f ./bochs/bochsrc
	
runqemu: $(ISO_FILE)
	@qemu-system-i386 --cdrom $<
	

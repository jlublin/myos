
# Makefile for MyOS

PLATFORM_PREFIX = arm-linux-gnueabi-
AS = $(PLATFORM_PREFIX)as
CC = $(PLATFORM_PREFIX)gcc
LD = $(PLATFORM_PREFIX)ld
OBJCOPY = $(PLATFORM_PREFIX)objcopy

.PHONY = all clean

LD_SCRIPT = memmap.ld
ELF = start.elf
BIN = start.bin
FLASH = flash.bin

all: flash.bin

clean:
	rm *.o *.elf *.bin

flash.bin: start.bin
	dd if=/dev/zero of=flash.bin bs=4096 count=4096
	dd if=start.bin of=flash.bin bs=4096 conv=notrunc

start.bin: start.elf
	$(OBJCOPY) -O binary start.elf start.bin

start.elf: start.o
	$(LD) -T $(LD_SCRIPT) -o start.elf start.o

start.o:
	$(AS) -o start.o start.S



# Makefile for MyOS

SHELL := /bin/bash

CROSS_COMPILE = arm-linux-gnueabi-
AS = $(CROSS_COMPILE)as --cpreproc
#CC = $(CROSS_COMPILE)gcc -nostartfiles -nostdlib -ffreestanding -c -O2 -mcpu=xscale
CC = $(CROSS_COMPILE)gcc -nostartfiles -nostdlib -ffreestanding -c -O1 -mcpu=xscale -Wall -Wextra
LD = $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy

.PHONY = all clean

LD_SCRIPT = memmap.ld
ELF = start.elf
BIN = start.bin
FLASH = flash.bin

all: flash.bin

clean:
	rm *.o *.elf *.bin

qemu: flash.bin
	qemu-system-arm -M connex -pflash flash.bin -nographic

qemu-gdb: flash.bin
	qemu-system-arm -s -S -M connex -pflash flash.bin -nographic &
	{ echo "target remote :1234"; cat; } | arm-linux-gdb

flash.bin: start.bin
	dd if=/dev/zero of=flash.bin bs=4096 count=4096
	dd if=start.bin of=flash.bin bs=4096 conv=notrunc

start.bin: start.elf
	$(OBJCOPY) -O binary start.elf start.bin

start.elf: start.o main.o memmap.ld
	$(LD) -T $(LD_SCRIPT) -o start.elf start.o main.o

start.o: start.S
	$(CC) -o start.o start.S

main.o: main.c
	$(CC) -o main.o main.c


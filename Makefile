# =============================================================================
# Variables

# Build tools
NASM = nasm -felf
CC = gcc
CFLAGS = 	-std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
LD = ld
LDFLAGS = -m elf_i386 -T link.ld
OBJCOPY = objcopy

# =============================================================================
# Tasks

all: clean build test

.tmp/kernel.o: src/kernel.c
	$(CC) $(CFLAGS) -c src/kernel.c -o .tmp/kernel.o

.tmp/boot.o: src/boot.asm
	@KSIZE=$$(stat -c %s .tmp/kernel.o); \
	TOTAL=$$((KSIZE + 512)); \
	$(NASM) src/boot.asm -o .tmp/boot.o -dN=$$TOTAL

.tmp/os.elf: .tmp/boot.o .tmp/kernel.o
	$(LD) $(LDFLAGS) .tmp/boot.o .tmp/kernel.o -o .tmp/os.elf

.tmp/os.bin: .tmp/os.elf
	$(OBJCOPY) -O binary .tmp/os.elf .tmp/os.bin

boot.img: .tmp/os.bin
	dd if=/dev/zero of=boot.img bs=1024 count=1440
	dd if=.tmp/os.bin of=boot.img conv=notrunc

build: boot.img

clean:
	rm -f *.img
	rm -rf .tmp
	mkdir -p .tmp

test: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -fda boot.img -monitor stdio -device VGA -s -S &
	gdb

.PHONY: all build clean test debug

# =============================================================================
# Variables

# Build tools
NASM = nasm -felf
CC = gcc
CFLAGS =  -std=c99 -m32 -O2 -ffreestanding -no-pie -fno-pie -mno-sse -fno-stack-protector
LD = ld
LDFLAGS = -m elf_i386 -T link.ld
OBJCOPY = objcopy

# Modules
KERNEL_OBJS = .tmp/kernel.o .tmp/vga_driver.o .tmp/memutils.o

# =============================================================================
# Tasks

all: clean build test

# Шаблонное правило для компиляции всех .c файлов в .o
.tmp/%.o: src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# boot.o теперь зависит от всех файлов ядра, чтобы посчитать их суммарный размер
.tmp/boot.o: src/boot.asm $(KERNEL_OBJS)
	@KSIZE=$$(cat $(KERNEL_OBJS) | wc -c); \
	TOTAL=$$((KSIZE + 512)); \
	$(NASM) src/boot.asm -o .tmp/boot.o -dN=$$TOTAL

.tmp/os.elf: .tmp/boot.o $(KERNEL_OBJS)
	$(LD) $(LDFLAGS) .tmp/boot.o $(KERNEL_OBJS) -o .tmp/os.elf

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
	qemu-system-i386 -cpu pentium2 -m 1g -drive file=boot.img,format=raw,if=floppy -monitor stdio -device VGA -display vnc=127.0.0.1:0 -audiodev none,id=snd0

debug: build
	qemu-system-i386 -cpu pentium2 -m 1g -drive file=boot.img,format=raw,if=floppy -monitor stdio -device VGA -display vnc=127.0.0.1:0 -s -S &
	gdb .tmp/os.elf

.PHONY: all build clean test debug
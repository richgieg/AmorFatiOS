#!/bin/bash

# Exit script if any command fails.
set -e

# Ensure bin directory exists.
mkdir -p bin

# Assemble the boot sector.
nasm -f bin -o bin/boot src/boot/boot.asm

# Compile the kernel.
gcc -masm=intel -m32 -ffreestanding -nostdlib -fno-pic -fno-pie -Wl,-no-pie -Wall -std=c11 \
    -Wl,--build-id=none -Wl,-T,kernel.ld -o bin/kernel \
    src/kernel/_start.c \
    src/kernel/kernel.c \
    src/kernel/vga.c \
    src/kernel/mm.c \
    src/kernel/console.c \
    src/kernel/process.c \
    src/kernel/idt.c \
    src/kernel/pic.c \
    src/kernel/ps2.c \
    src/kernel/timer.c \
    src/kernel/keyboard.c \
    src/kernel/mouse.c \
    src/kernel/pci.c \
    src/kernel/i82545em.c \
    src/kernel/string.c \
    src/kernel/bugcheck.c

# Write boot sector and kernel to virtual floppy disk file.
sudo dd if=/dev/zero of=bin/os.flp bs=512 count=2880
sudo dd if=bin/boot of=bin/os.flp conv=notrunc
sudo dd if=bin/kernel of=bin/os.flp seek=1 conv=notrunc

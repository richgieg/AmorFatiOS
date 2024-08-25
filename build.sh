#!/bin/bash

# Exit script if any command fails.
set -e

# Ensure bin directory exists.
mkdir -p bin

# Assemble the boot sector.
nasm -f bin -o bin/boot src/boot/boot.s

# Compile the kernel.
gcc -masm=intel -m32 -ffreestanding -nostdlib -Wall -std=c11 \
    -Wl,-Trodata=0x23000 -Wl,-Ttext=0x21000 -Wl,-Tdata=0x24000 -o bin/kernel \
    src/kernel/_start.c \
    src/kernel/kernel.c \
    src/kernel/vga.c \
    src/kernel/idt.c \
    src/kernel/pic.c \
    src/kernel/ps2.c \
    src/kernel/timer.c \
    src/kernel/keyboard.c \
    src/kernel/mouse.c

# Write boot sector and kernel to virtual floppy disk file.
sudo dd if=/dev/zero of=bin/os.flp bs=512 count=2880
sudo dd if=bin/boot of=bin/os.flp conv=notrunc
sudo dd if=bin/kernel of=bin/os.flp seek=1 conv=notrunc
#!/bin/bash

# Exit script if any command fails.
set -e

# Ensure bin directory exists.
mkdir -p bin

# Build the bootloader image.
nasm -f bin -o bin/boot boot/boot.asm

# Build the kernel image.
gcc -masm=intel -m32 -ffreestanding -nostdlib -fno-pic -fno-pie -Wl,-no-pie -Wall -std=c11 \
    -I./kernel/include -Wl,--build-id=none -Wl,-T,kernel.ld -o bin/kernel \
    kernel/_start.c \
    kernel/device/i82545em.c \
    kernel/device/keyboard.c \
    kernel/device/mouse.c \
    kernel/device/pci.c \
    kernel/device/pic.c \
    kernel/device/ps2.c \
    kernel/device/timer.c \
    kernel/device/vga.c \
    kernel/lib/string.c \
    kernel/program/counter.c \
    kernel/program/memdump.c \
    kernel/program/memmap.c \
    kernel/bugcheck.c \
    kernel/console.c \
    kernel/gdt.c \
    kernel/idt.c \
    kernel/kernel.c \
    kernel/process.c \
    kernel/tss.c

# Build the userspace image.
gcc -masm=intel -m32 -ffreestanding -nostdlib -fno-pic -fno-pie -Wl,-no-pie -Wall -std=c11 \
    -I./userspace/include -Wl,--build-id=none -Wl,-T,kernel.ld -o bin/userspace \
    userspace/_start.c \
    userspace/userspace.c

# Write bootloader, kernel, and userspace images to virtual floppy disk file.
sudo dd if=/dev/zero of=bin/os.flp bs=512 count=2880
sudo dd if=bin/boot of=bin/os.flp conv=notrunc
sudo dd if=bin/kernel of=bin/os.flp bs=512 seek=36 conv=notrunc
sudo dd if=bin/userspace of=bin/os.flp bs=512 seek=576 conv=notrunc

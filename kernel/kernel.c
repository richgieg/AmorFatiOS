#include <kernel.h>
#include <device/vga.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <device/pic.h>
#include <device/timer.h>
#include <device/ps2.h>
#include <device/keyboard.h>
#include <device/mouse.h>
#include <device/pci.h>
#include <console.h>
#include <process.h>

void kernel_init(void) {
    vga_init();
    console_init();
    process_init();
    gdt_init();
    idt_init();
    tss_init();
    pic_init();
    timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();
    pci_init();

    // Enable interrupts
    __asm__("sti");

    // Create the initial userspace process.
    process_create((void *)0x91000);

    console_puts("AmorFatiOS v0.0.1 - Kernel\n");

    while (1) {
        // TODO: Do work here as needed.
        __asm__("hlt");
    }
}

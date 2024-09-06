#include <kernel.h>
#include <device/vga.h>
#include <idt.h>
#include <device/pic.h>
#include <device/timer.h>
#include <device/ps2.h>
#include <device/keyboard.h>
#include <device/mouse.h>
#include <device/pci.h>
#include <console.h>
#include <process.h>
#include <program/memdump.h>
#include <program/memmap.h>
#include <program/counter.h>

void kernel_init(void) {
    vga_init();
    console_init();
    process_init();
    idt_init();
    pic_init();
    timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();
    pci_init();

    // Enable interrupts
    __asm__("sti");

    process_create(memdump);
    process_create(memmap);
    process_create(counter);

    console_puts("AmorFatiOS v0.0.1\n");

    while (1) {
        // TODO: Do work here as needed.
        __asm__("hlt");
    }
}
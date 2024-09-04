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

static void process3_start() {
    console_dbg_puts("Process 3 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 3);
        counter++;
    }
}

static void process4_start() {
    console_dbg_puts("Process 4 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 4);
        counter++;
    }
}

static void process5_start() {
    console_dbg_puts("Process 5 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 5);
        counter++;
    }
}

static void process6_start() {
    console_dbg_puts("Process 6 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 6);
        counter++;
    }
}

static void process7_start() {
    console_dbg_puts("Process 7 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 7);
        counter++;
    }
}

void kernel_init(void) {
    vga_init();
    console_init();
    idt_init();
    pic_init();
    // timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();
    pci_init();

    // Enable interrupts
    __asm__("sti");

    // mm_show_mmap();

    process_init();
    process_create(memdump);
    process_create(memmap);
    process_create(process3_start);
    process_create(process4_start);
    process_create(process5_start);
    process_create(process6_start);
    process_create(process7_start);
    
    timer_init();

    console_puts("AmorFatiOS v0.0.1\n");
    console_dbg_puts("Process 0 started\n");

    while (1) {
        // TODO: Do work here as needed.
        __asm__("hlt");
    }
}

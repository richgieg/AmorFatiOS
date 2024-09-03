#include "kernel.h"
#include "vga.h"
#include "mm.h"
#include "scheduler.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "ps2.h"
#include "keyboard.h"
#include "mouse.h"
#include "pci.h"
#include "console.h"
#include "process.h"

static void process1_start() {
    console_dbg_puts("Process 1 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 1);
        counter++;
        process_switch();
    }
}

static void process2_start() {
    console_dbg_puts("Process 2 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 2);
        counter++;
        process_switch();
    }
}

static void process3_start() {
    console_dbg_puts("Process 3 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 3);
        counter++;
        process_switch();
    }
}

static void process4_start() {
    console_dbg_puts("Process 4 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 4);
        counter++;
        process_switch();
    }
}

static void process5_start() {
    console_dbg_puts("Process 5 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 5);
        counter++;
        process_switch();
    }
}

static void process6_start() {
    console_dbg_puts("Process 6 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 6);
        counter++;
        process_switch();
    }
}

static void process7_start() {
    console_dbg_puts("Process 7 started\n");
    u32 counter = 0;
    while (1) {
        console_putdw_at(counter, 0, 7);
        counter++;
        process_switch();
    }
}

void kernel_init(void) {
    vga_init();
    console_init();
    mm_init();
    scheduler_init();
    idt_init();
    pic_init();
    timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();
    pci_init();

    // Enable interrupts
    __asm__("sti");

    // mm_show_mdump();
    // mm_show_mmap();

    process_init();
    process_create(process1_start);
    process_create(process2_start);
    process_create(process3_start);
    process_create(process4_start);
    process_create(process5_start);
    process_create(process6_start);
    process_create(process7_start);
    
    console_puts("AmorFatiOS v0.0.1\n");
    console_dbg_puts("Process 0 started\n");
    while (1) {
        process_switch();
        // __asm__("hlt");
    }
}

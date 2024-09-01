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
#include "process.h"

static void process1_start() {
    vga_puts("Process 1 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 1 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

static void process2_start() {
    vga_puts("Process 2 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 2 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

static void process3_start() {
    vga_puts("Process 3 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 3 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

static void process4_start() {
    vga_puts("Process 4 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 4 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

static void process5_start() {
    vga_puts("Process 5 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 5 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

static void process6_start() {
    vga_puts("Process 6 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 6 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

static void process7_start() {
    vga_puts("Process 7 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 7 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

void kernel_init(void) {
    vga_init();
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

    // vga_puts("AmorFatiOS v0.0.1\n");

    // mm_show_mdump();
    // mm_show_mmap();

    // __asm__("cli");

    process_init();
    process_create(process1_start);
    process_create(process2_start);
    process_create(process3_start);
    process_create(process4_start);
    process_create(process5_start);
    process_create(process6_start);
    process_create(process7_start);
    
    vga_puts("Process 0 started\n");
    while (1) {
        // for (int i = 0; i < 1000000000; i++);
        // vga_puts("Process 0 switching\n");
        process_switch();
        // __asm__("hlt");
    }
}

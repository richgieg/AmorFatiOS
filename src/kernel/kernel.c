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

static void process0_start() {
    vga_puts("Process 0 started\n");
    while (1) {
        process_switch();
        // __asm__("hlt");
    }
}

static void process1_start() {
    vga_puts("Process 1 started\n");
    while (1) {
        process_switch();
        // __asm__("hlt");
    }
}

void kernel_init(void) {
    vga_init();
    // mm_init();
    // scheduler_init();
    // idt_init();
    // pic_init();
    // timer_init();
    // ps2_init();
    // keyboard_init();
    // mouse_init();
    // pci_init();

    // vga_puts("AmorFatiOS v0.0.1\n");

    // mm_show_mdump();
    // mm_show_mmap();

    __asm__("cli");

    process_create(process0_start);
    process_create(process1_start);
    process_switch();
}

void kernel_idle(void) {
    while (1) {
        __asm__("hlt");
    }
}

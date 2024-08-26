#include "kernel.h"
#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "ps2.h"
#include "keyboard.h"
#include "mouse.h"
#include "memdump.h"

void kernel_init(void) {
    vga_init();
    idt_init();
    pic_init();
    timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();

    // vga_puts("AmorFatiOS v0.0.1\n");

    // md_next();
}

void kernel_idle(void) {
    while (1) {
        __asm__("hlt");
    }
}

#include "kernel.h"
#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "ps2.h"
#include "keyboard.h"
#include "mouse.h"

void kernel_init(void) {
    vga_init();
    idt_init();
    pic_init();
    timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();

    // vga_puts("AmorFatiOS v0.0.1\n");

    volatile uint8_t *memory_ptr = (volatile uint8_t *)0;

    for (int k = 0; k < 25; k++) {
        vga_putdw((uint32_t)memory_ptr);
        vga_putc(' ');
        vga_putc(' ');
        vga_putc(' ');
        vga_putc(' ');

        for (int i = 0; i < 16; i++) {
            vga_putb(*memory_ptr);
            vga_putc(' ');
            memory_ptr++;
        }

        vga_putc(' ');
        vga_putc(' ');
        vga_putc(' ');

        memory_ptr -= 16;
        for (int i = 0; i < 16; i++) {
            vga_writec(*memory_ptr);
            memory_ptr++;
        }
        if (k < 24) {
            vga_putc('\n');
        }
    }
}

void kernel_idle(void) {
    while (1) {
        __asm__("hlt");
    }
}

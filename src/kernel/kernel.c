#include "kernel.h"
#include "vga.h"
#include "idt.h"
#include "pic.h"
#include "timer.h"
#include "ps2.h"
#include "keyboard.h"
#include "mouse.h"
#include "memdump.h"

typedef struct {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
    uint32_t extended_attributes;
} __attribute__((packed)) mem_entry;

void kernel_init(void) {
    vga_init();
    idt_init();
    pic_init();
    timer_init();
    ps2_init();
    keyboard_init();
    mouse_init();

    // vga_puts("AmorFatiOS v0.0.1\n");

    // md_show();

    volatile mem_entry *e = (mem_entry *)0x10000;

    while (e->base_address || e->length || e->type || e->extended_attributes) {
        vga_putqw(e->base_address);
        vga_putc(' ');
        vga_putqw(e->length);
        vga_putc(' ');
        vga_putdw(e->type);
        vga_putc(' ');
        vga_putdw(e->extended_attributes);
        vga_putc('\n');
        e++;
    }
}

void kernel_idle(void) {
    while (1) {
        __asm__("hlt");
    }
}

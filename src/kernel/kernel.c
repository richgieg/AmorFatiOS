#include "idt.h"
#include "pic.h"
#include "console.h"

void _start(void) {
    console_init();
    console_clear();
    console_print_at("AmorFatiOS v0.0.1", 0, 0);

    idt_init();
    pic_init();
    pic_unmask_irq(1); // enable keyboard interrupts

    while (1) {
        __asm__("hlt");
    }
}

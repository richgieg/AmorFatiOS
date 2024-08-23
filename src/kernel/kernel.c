#include "idt.h"
#include "pic.h"
#include "console.h"

const char greeting[] = "AmorFatiOS v0.0.1";

void _start(void) {
    console_set_bg_color(3);
    console_set_text_color(15);
    console_print_at(greeting, 0, 0);

    idt_init();
    pic_init();
    pic_unmask_irq(1); // enable keyboard interrupts

    while (1) {
        __asm__("hlt");
    }
}

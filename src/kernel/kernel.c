#include "idt.h"
#include "pic.h"
#include "console.h"

const char greeting[] = "AmorFatiOS v0.0.1";

void _start(void) {
    console_init();
    console_set_bg_color(CONSOLE_COLOR_CYAN);
    console_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_print_at(greeting, 0, 0);

    idt_init();
    pic_init();
    pic_unmask_irq(1); // enable keyboard interrupts

    while (1) {
        __asm__("hlt");
    }
}

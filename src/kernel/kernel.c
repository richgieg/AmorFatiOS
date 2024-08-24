#include "idt.h"
#include "pic.h"
#include "console.h"
#include "mouse.h"

void kernel_init(void) {
    console_init();
    console_clear();
    console_print_at("AmorFatiOS v0.0.1", 0, 0);

    idt_init();
    pic_init();
    pic_unmask_irq(0);
    pic_unmask_irq(1); // enable keyboard interrupts

    mouse_init();

    while (1) {
        __asm__("hlt");
    }
}

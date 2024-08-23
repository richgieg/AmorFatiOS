#include "init_idt.h"
#include "pic.h"

char *const screen = (char *)0xb8000;
const char greeting[] = "AmorFatiOS v0.0.1";

void _start(void) {
    for (int i = 0; i < 80 * 25; i++) {
        screen[i * 2 + 1] = 0x3f;
    }

    for (int i = 0; i < sizeof(greeting); i++) {
        screen[i * 2] = greeting[i];
    }

    init_idt();
    pic_init();
    pic_unmask_irq(1);

    while (1) {
        __asm__("hlt");
    }
}

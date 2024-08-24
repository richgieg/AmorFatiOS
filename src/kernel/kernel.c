#include "idt.h"
#include "pic.h"
#include "console.h"
#include "port.h"

void kernel_init(void) {
    console_init();
    console_clear();
    console_print_at("AmorFatiOS v0.0.1", 0, 0);

    idt_init();
    pic_init();
    pic_unmask_irq(0);
    pic_unmask_irq(1); // enable keyboard interrupts
    pic_unmask_irq(2);
    pic_unmask_irq(3);
    pic_unmask_irq(4);
    pic_unmask_irq(5);
    pic_unmask_irq(6);
    pic_unmask_irq(7);
    pic_unmask_irq(8);
    pic_unmask_irq(9);
    pic_unmask_irq(10);
    pic_unmask_irq(11);
    pic_unmask_irq(12);
    pic_unmask_irq(13);
    pic_unmask_irq(14);
    pic_unmask_irq(15);

    // outb(0x64, 0x20);
    // uint8_t status = inb(0x64);
    // status &= 0xdf;
    // status |= 0x02;
    // outb(0x64, 0x60);
    // outb(0x60, status);

    while (1) {
        __asm__("hlt");
    }
}

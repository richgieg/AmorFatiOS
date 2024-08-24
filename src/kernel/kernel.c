#include "idt.h"
#include "pic.h"
#include "console.h"
#include "port.h"

#define STATUS_PORT 0x64
#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

#define ENABLE_AUX_PORT 0xa8
#define ENABLE_KBD_PORT 0xae
#define WRITE_AUX_DEVICE 0xd4
#define RESET_MOUSE 0xff
#define ENABLE_DATA 0xf4

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

    while ((inb(STATUS_PORT) & 2) != 0); // wait_input_clear
    outb(COMMAND_PORT, ENABLE_AUX_PORT);

    while ((inb(STATUS_PORT) & 2) != 0); // wait_input_clear
    outb(COMMAND_PORT, WRITE_AUX_DEVICE);

    while ((inb(STATUS_PORT) & 2) != 0); // wait_input_clear
    outb(DATA_PORT, RESET_MOUSE);

    while ((inb(STATUS_PORT) & 1) != 1); // wait_output_full
    inb(DATA_PORT);

    while ((inb(STATUS_PORT) & 1) != 1); // wait_output_full
    inb(DATA_PORT);

    while ((inb(STATUS_PORT) & 2) != 0); // wait_input_clear
    outb(COMMAND_PORT, WRITE_AUX_DEVICE);

    while ((inb(STATUS_PORT) & 2) != 0); // wait_input_clear
    outb(DATA_PORT, ENABLE_DATA);

    while (1) {
        __asm__("hlt");
    }
}

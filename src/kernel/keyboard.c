#include "keyboard.h"
#include "console.h"
#include "port.h"
#include "idt.h"
#include "pic.h"

#define STATUS_PORT 0x64
#define COMMAND_PORT 0x64
#define DATA_PORT 0x60

#define ENABLE_KBD_PORT 0xae
#define RESET_KBD 0xff
#define ENABLE_SCANNING 0xf4

__attribute__((naked))
static void interrupt_service_routine(void) {
    char *p = " ";
    p[0] = inb(0x60);
    console_print_at(p, 2, 2);
    outb(0x20, 0x20); // send EOI command to primary PIC
    __asm__("iret");
}

void keyboard_init(void) {
    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(COMMAND_PORT, ENABLE_KBD_PORT);

    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(DATA_PORT, RESET_KBD);

    // Read ACK (0xfa)
    while ((inb(STATUS_PORT) & 1) == 0); // wait_output_full
    inb(DATA_PORT);

    // Read self-test pass (0xaa)
    while ((inb(STATUS_PORT) & 1) == 0); // wait_output_full
    inb(DATA_PORT);

    while (inb(STATUS_PORT) & 2); // wait_input_clear
    outb(DATA_PORT, ENABLE_SCANNING);

    idt_set_descriptor(IRQ1_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(1);
}

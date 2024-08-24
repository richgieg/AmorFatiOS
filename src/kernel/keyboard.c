#include "keyboard.h"
#include "console.h"
#include "port.h"
#include "idt.h"
#include "pic.h"

__attribute__((naked))
static void interrupt_service_routine(void) {
    char *p = " ";
    p[0] = inb(0x60);
    console_print_at(p, 2, 2);
    outb(0x20, 0x20); // send EOI command to primary PIC
    __asm__ volatile ("iret");
}

void keyboard_init(void) {
    idt_set_descriptor(IRQ1_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(1);
}

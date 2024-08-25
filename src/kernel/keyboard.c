#include "keyboard.h"
#include "vga.h"
#include "port.h"
#include "idt.h"
#include "ps2.h"
#include "pic.h"

__attribute__((naked))
static void interrupt_service_routine(void) {
    char *p = " ";
    p[0] = inb(PS2_DATA_PORT);
    vga_print_at(p, 2, 2);
    outb(PIC1_COMMAND, PIC_EOI);
    __asm__("iret");
}

void keyboard_init(void) {
    idt_set_descriptor(IRQ1_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(1);
}

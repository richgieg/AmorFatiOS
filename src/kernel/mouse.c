#include "mouse.h"
#include "port.h"
#include "idt.h"
#include "ps2.h"
#include "pic.h"
#include "vga.h"

static void interrupt_service_routine(void) {
    u8 value = inb(PS2_DATA_PORT);
    if (value != PS2_DEV_ACK) {
        vga_putb(value);
        vga_putc(' ');
    }
    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC2_COMMAND, PIC_EOI);
}

void mouse_init(void) {
    idt_set_irq_handler(12, interrupt_service_routine);
    pic_unmask_irq(2);
    pic_unmask_irq(12);
}

#include "mouse.h"
#include "port.h"
#include "idt.h"
#include "ps2.h"
#include "pic.h"
#include "vga.h"

static void interrupt_service_routine(void) {
    uint8_t value = inb(PS2_DATA_PORT);
    if (value != PS2_DEV_ACK) {
        vga_putb(value);
        vga_putc(' ');
    }
    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC2_COMMAND, PIC_EOI);
}

__attribute__((naked))
static void interrupt_service_routine_stub(void) {
    interrupt_service_routine();
    __asm__("iret");
}

void mouse_init(void) {
    idt_set_descriptor(IRQ12_INTERRUPT, interrupt_service_routine_stub, 0x8e);
    pic_unmask_irq(2);
    pic_unmask_irq(12);
}

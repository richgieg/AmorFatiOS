#include "mouse.h"
#include "vga.h"
#include "port.h"
#include "idt.h"
#include "ps2.h"
#include "pic.h"

__attribute__((naked))
static void interrupt_service_routine(void) {
    uint8_t value = inb(PS2_DATA_PORT);
    vga_print_char_at(value, 24, 2);
    outb(PIC1_COMMAND, PIC_EOI);
    outb(PIC2_COMMAND, PIC_EOI);
    __asm__("iret");
}

void mouse_init(void) {
    idt_set_descriptor(IRQ12_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(2);
    pic_unmask_irq(12);
}

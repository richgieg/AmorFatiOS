#include "timer.h"
#include "port.h"
#include "idt.h"
#include "pic.h"

__attribute__((naked))
static void interrupt_service_routine(void) {
    // TODO: Do something here.
    outb(PIC1_COMMAND, PIC_EOI);
    __asm__("iret");
}

void timer_init(void) {
    idt_set_descriptor(IRQ0_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(0);
}

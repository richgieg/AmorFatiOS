#include "timer.h"
#include "port.h"
#include "idt.h"
#include "pic.h"

static void interrupt_service_routine(void) {
    // TODO: Do something here.
    outb(PIC1_COMMAND, PIC_EOI);
}

__attribute__((naked))
static void interrupt_service_routine_stub(void) {
    interrupt_service_routine();
    __asm__("jmp return_from_interrupt");
}

void timer_init(void) {
    idt_set_descriptor(IRQ0_INTERRUPT, interrupt_service_routine_stub, 0x8e);
    pic_unmask_irq(0);
}

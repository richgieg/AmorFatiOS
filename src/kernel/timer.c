#include "timer.h"
#include "console.h"
#include "port.h"
#include "idt.h"
#include "pic.h"

char buf[1] = "";

__attribute__((naked))
static void interrupt_service_routine(void) {
    console_print_at(buf, 0, 2);
    buf[0]++;
    outb(0x20, 0x20); // send EOI command to primary PIC
    __asm__ volatile ("iret");
}

void timer_init(void) {
    idt_set_descriptor(IRQ0_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(0);
}

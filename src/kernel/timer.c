#include "timer.h"
#include "vga.h"
#include "port.h"
#include "idt.h"
#include "pic.h"

char counter = 0;

__attribute__((naked))
static void interrupt_service_routine(void) {
    vga_print_char_at(counter, 0, 2);
    counter++;
    outb(PIC1_COMMAND, PIC_EOI);
    __asm__("iret");
}

void timer_init(void) {
    idt_set_descriptor(IRQ0_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(0);
}

#include "timer.h"
#include "port.h"
#include "idt.h"
#include "pic.h"
#include "scheduler.h"

static void interrupt_service_routine(void) {
    scheduler_update();
    outb(PIC1_COMMAND, PIC_EOI);
}

void timer_init(void) {
    idt_set_irq_handler(0, interrupt_service_routine);
    pic_unmask_irq(0);
}

#include "timer.h"
#include "port.h"
#include "idt.h"
#include "pic.h"
#include "scheduler.h"

#define BASE_FREQ_HZ 1193182
#define DESIRED_FREQ_HZ 100

static void interrupt_service_routine(void) {
    outb(PIC1_COMMAND, PIC_EOI);
    scheduler_update();
}

void timer_init(void) {
    u16 divisor = BASE_FREQ_HZ / DESIRED_FREQ_HZ;
    u8 low = divisor;
    u8 high = divisor >> 8;
    outb(0x43, 0x36);
    outb(0x40, low);
    outb(0x40, high);

    idt_set_irq_handler(0, interrupt_service_routine);
    pic_unmask_irq(0);
}

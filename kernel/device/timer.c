#include <device/timer.h>
#include <port.h>
#include <idt.h>
#include <device/pic.h>
#include <process.h>

#define BASE_FREQ_HZ 1193182
#define DESIRED_FREQ_HZ 1000

static u32 ticks;

static void interrupt_service_routine(void) {
    outb(PIC1_COMMAND, PIC_EOI);
    ticks++;

    // Switch processes 200 times per second.
    if (ticks % 5 == 0) {
        process_switch(PROCESS_STATE_RUNNABLE);
    }
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

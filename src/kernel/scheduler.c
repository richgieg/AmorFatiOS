#include "scheduler.h"
#include "types.h"
#include "vga.h"

static u32 ticks;

void scheduler_init(void) {

}

void scheduler_update(void) {
    // TODO: Update accounting for current process/thread.
    ticks++;
    if (ticks % 0x100 == 0) {
        vga_puts("scheduler_update calls: ");
        vga_putdw(ticks);
        vga_putc('\n');
    }
}

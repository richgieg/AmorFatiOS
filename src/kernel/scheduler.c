#include "scheduler.h"
#include "types.h"
#include "console.h"

static u32 ticks;

void scheduler_init(void) {

}

void scheduler_update(void) {
    // TODO: Update accounting for current process/thread.
    ticks++;
    if (ticks % 0x100 == 0) {
        console_puts("scheduler_update calls: ");
        console_putdw(ticks);
        console_putc('\n');
    }
}

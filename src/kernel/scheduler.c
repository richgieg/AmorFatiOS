#include "scheduler.h"
#include "types.h"
#include "console.h"
#include "process.h"

static u32 ticks;

void scheduler_init(void) {

}

void scheduler_update(void) {
    // TODO: Update accounting for current process/thread.
    ticks++;
    if (ticks == 1) {
        ticks = 0;
        // console_dbg_puts("scheduler_update calls: ");
        // console_dbg_putdw(ticks);
        // console_dbg_putc('\n');
        process_switch();
    }
}

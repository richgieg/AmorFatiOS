#include <program/counter.h>
#include <types.h>
#include <console.h>

void counter(void) {
    u32 count = 0;
    while (1) {
        console_putdw_at(count, 0, 0);
        count++;
    }
}

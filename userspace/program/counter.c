#include <program/counter.h>
#include <syscall.h>

void counter(void) {
    int count = 0;
    
    while (1) {
        sys_console_putdw_at(count, 0, 0);
        count++;
    }
}

#include <program/counter.h>
#include <sys.h>

void counter(void) {
    int count = 0;

    for (int i = 0; i <= 0x00ffffff; i++) {
        sys_console_putdw_at(count, 0, 0);
        count++;
    }

    sys_process_exit();
}

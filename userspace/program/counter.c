#include <program/counter.h>
#include <sys.h>

void counter(void) {
    int count = 0;

    u8 col = sys_console_get_column();
    u8 row = sys_console_get_row();

    for (int i = 0; i <= 0x00ffffff; i++) {
        sys_console_set_pos(col, row);
        sys_console_putdw(count);
        count++;
    }

    sys_console_putc('\n');
    sys_process_exit();
}

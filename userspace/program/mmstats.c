#include <program/mmstats.h>
#include <sys.h>

void mmstats(void) {
    struct mm_stats stats;
    sys_mm_get_stats(&stats);

    sys_console_puts("num_8b_allocs: ");
    sys_console_putdw(stats.num_8b_allocs);
    sys_console_putc('\n');

    sys_console_puts("num_64b_allocs: ");
    sys_console_putdw(stats.num_64b_allocs);
    sys_console_putc('\n');

    sys_console_puts("num_512b_allocs: ");
    sys_console_putdw(stats.num_512b_allocs);
    sys_console_putc('\n');

    sys_console_puts("num_4k_allocs: ");
    sys_console_putdw(stats.num_4k_allocs);
    sys_console_putc('\n');

    sys_console_puts("num_32k_allocs: ");
    sys_console_putdw(stats.num_32k_allocs);
    sys_console_putc('\n');

    sys_console_puts("num_256k_allocs: ");
    sys_console_putdw(stats.num_256k_allocs);
    sys_console_putc('\n');

    sys_console_puts("num_2m_allocs: ");
    sys_console_putdw(stats.num_2m_allocs);
    sys_console_putc('\n');

    sys_process_exit();
}

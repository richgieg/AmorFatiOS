#include <program/mmstats.h>
#include <sys.h>

void mmstats(void) {
    struct mm_stats stats;
    sys_mm_get_stats(&stats);

    sys_console_puts("page size (bytes):    ");
    sys_console_putdw(stats.page_size);
    sys_console_putc('\n');

    sys_console_puts("chunk size (bytes):   ");
    sys_console_putdw(stats.chunk_size);
    sys_console_putc('\n');

    sys_console_puts("num_pages_allocated:  ");
    sys_console_putdw(stats.num_pages_allocated);
    sys_console_putc('\n');

    sys_console_puts("num_chunks_allocated: ");
    sys_console_putdw(stats.num_chunks_allocated);
    sys_console_putc('\n');

    sys_process_exit();
}

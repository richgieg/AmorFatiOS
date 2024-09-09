#include <userspace.h>
#include <sys.h>
#include <program/memmap.h>
#include <program/memdump.h>
#include <program/counter.h>

void userspace_init(void) {
    sys_console_puts("AmorFatiOS v0.0.1 - Userspace\n");
    sys_process_create(memmap);
    sys_process_create(memdump);
    sys_process_create(counter);

    while (1) {
        // TODO: Do some userspace stuff here.
    }
}

#include <userspace.h>
#include <sys.h>
#include <program/counter.h>
#include <program/echo.h>
#include <program/memdump.h>

void userspace_init(void) {
    // sys_console_putdw_at(0xdeadbeef, 0, 0);
    sys_console_putqw(0xdeadbeef01234567);
    sys_console_putqw_at(0xdeadbeef01234567, 0, 5);
    sys_process_create(counter);
    sys_process_create(echo);
    sys_process_create(memdump);

    while (1) {
        // TODO: Do some userspace stuff here.
    }
}

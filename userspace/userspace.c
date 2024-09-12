#include <userspace.h>
#include <sys.h>
#include <program/shell.h>
#include <program/memmap.h>
#include <program/memdump.h>
#include <program/counter.h>

void userspace_init(void) {
    sys_process_create_in_console(shell, 0);
    sys_process_create_in_console(shell, 1);
    sys_process_create_in_console(shell, 2);
    sys_process_create_in_console(shell, 3);
    sys_process_create_in_console(shell, 4);
    sys_process_create_in_console(shell, 5);
    sys_process_create_in_console(shell, 6);
    sys_process_create_in_console(shell, 7);

    while (1) {
        // TODO: Do some userspace stuff here.
    }
}

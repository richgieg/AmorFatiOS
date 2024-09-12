#include <userspace.h>
#include <sys.h>
#include <program/shell.h>
#include <program/memmap.h>
#include <program/memdump.h>
#include <program/counter.h>

void userspace_init(void) {
    int num_consoles = sys_console_get_num_consoles();

    for (int i = 0; i < num_consoles; i++) {
        sys_process_create_in_console(shell, i);
    }

    while (1) {
        // TODO: Do some userspace stuff here.
    }
}

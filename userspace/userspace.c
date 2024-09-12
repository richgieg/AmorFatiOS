#include <userspace.h>
#include <sys.h>
#include <program/shell_spawner.h>

void userspace_init(void) {
    int num_consoles = sys_console_get_num_consoles();

    for (int i = 0; i < num_consoles; i++) {
        sys_process_create_in_console(shell_spawner, i);
    }

    while (1) {
        // TODO: Do some userspace stuff here.
    }
}

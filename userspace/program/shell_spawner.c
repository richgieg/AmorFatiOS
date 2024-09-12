#include <program/shell_spawner.h>
#include <sys.h>
#include <program/shell.h>

void shell_spawner(void) {
    while (true) {
        int pid = sys_process_create(shell);
        sys_process_wait_for_exit(pid);
    }
}

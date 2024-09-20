#include <program/proctree.h>
#include <sys.h>

static void print_tree(int pid, int level) {
    for (int i = 0; i < level; i++) {
        sys_console_puts("--");
    }
    sys_console_putdw(pid);
    sys_console_puts("\n");

    int child_pids[64];
    int count;

    sys_process_get_child_pids(pid, child_pids, sizeof(child_pids), &count);

    for (int i = 0; i < count; i++) {
        print_tree(child_pids[i], level + 1);
    }
}

void proctree(void) {
    int top_level_pids[64];
    int count;

    sys_process_get_child_pids(-1, top_level_pids, sizeof(top_level_pids), &count);

    for (int i = 0; i < count; i++) {
        print_tree(top_level_pids[i], 0);
    }

    sys_process_exit();
}

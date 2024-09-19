#include <userspace.h>
#include <sys.h>
#include <stdio.h>
#include <program/login.h>

void userspace_init(void) {
    while (true) {
        puts("AmorFatiOS v0.0.1 (vt");
        sys_console_putb(sys_process_get_console_index() + 1);
        puts(")\n\n");
        int pid = sys_process_create(login);
        sys_process_wait_for_exit(pid);
    }
}

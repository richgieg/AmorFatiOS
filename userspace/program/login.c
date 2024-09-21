#include <program/login.h>
#include <stdio.h>
#include <string.h>
#include <sys.h>
#include <program/shell.h>

void login(void) {
    char buf[2048];
    bool is_authenticated = false;

    while (!is_authenticated) {
        puts("login: ");
        gets(buf);
        if (strcmp(buf, "root") == 0) {
            is_authenticated = true;
        } else {
            puts("\nLogin incorrect\n");
        }
    }

    int pid = sys_process_create_ex(shell, false);
    sys_process_wait_for_exit(pid);
    sys_console_clear();
    sys_process_exit();
}

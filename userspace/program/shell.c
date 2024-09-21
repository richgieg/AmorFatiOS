#include <program/shell.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys.h>
#include <program/counter.h>
#include <program/login.h>
#include <program/memdump.h>
#include <program/memmap.h>
#include <program/proctree.h>

static char * trim(char *str);

void shell(void) {
    char buf[2048];

    // puts("AmorFatiOS Shell v0.0.1\n\n");

    while (true) {
        puts("$> ");
        gets(buf);

        char *trimmed = trim(buf);
        if (strcmp(trimmed, "help") == 0) {
            puts("The \"help\" command is coming soon!\n");
        } else if (strcmp(trimmed, "clear") == 0) {
            sys_console_clear();
        } else if (strcmp(trimmed, "exit") == 0) {
            sys_process_exit();
        } else if (strcmp(trimmed, "counter") == 0) {
            int pid = sys_process_create(counter);
            if (pid != -1) {
                sys_process_wait_for_exit(pid);
            } else {
                sys_console_puts("Failed to create process.\n");
            }
        } else if (strcmp(trimmed, "login") == 0) {
            int pid = sys_process_create(login);
            if (pid != -1) {
                sys_process_wait_for_exit(pid);
            } else {
                sys_console_puts("Failed to create process.\n");
            }
        } else if (strcmp(trimmed, "memmap") == 0) {
            int pid = sys_process_create(memmap);
            if (pid != -1) {
                sys_process_wait_for_exit(pid);
            } else {
                sys_console_puts("Failed to create process.\n");
            }
        } else if (strcmp(trimmed, "memdump") == 0) {
            int pid = sys_process_create(memdump);
            if (pid != -1) {
                sys_process_wait_for_exit(pid);
            } else {
                sys_console_puts("Failed to create process.\n");
            }
        } else if (strcmp(trimmed, "proctree") == 0) {
            int pid = sys_process_create(proctree);
            if (pid != -1) {
                sys_process_wait_for_exit(pid);
            } else {
                sys_console_puts("Failed to create process.\n");
            }
        } else if (strcmp(trimmed, "shell") == 0) {
            int pid = sys_process_create_ex(shell, false);
            if (pid != -1) {
                sys_process_wait_for_exit(pid);
            } else {
                sys_console_puts("Failed to create process.\n");
            }
        } else if (strlen(trimmed) != 0) {
            puts(trimmed);
            puts(": command not found\n");
        }
    }
}

static char * trim(char *str) {
    // Trim leading space.
    while (isspace(*str)) str++;

    // Is all space?
    if (*str == 0) {
        return str;
    }

    // Trim trailing space.
    char *end = str + strlen(str) - 1;
    while (isspace(*end)) end--;

    // Write new terminator.
    *(end + 1) = '\0';

    return str;
}

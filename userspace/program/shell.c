#include <program/shell.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <sys.h>

static char * trim(char *str);

void shell(void) {
    char buf[2048];

    puts("AmorFatiOS Shell v0.0.1\n\n");

    while (true) {
        puts("$> ");
        gets(buf);

        char *trimmed = trim(buf);
        if (strcmp(trimmed, "help") == 0) {
            puts("The \"help\" command is coming soon!\n");
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

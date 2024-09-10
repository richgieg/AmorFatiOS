#include <program/shell.h>
#include <stdio.h>

void shell(void) {
    char buf[2048];

    puts("AmorFatiOS Shell v0.0.1\n\n");

    while (true) {
        puts("$> ");
        gets(buf);
        puts("You wrote: ");
        puts(buf);
        puts("\n");
    }
}

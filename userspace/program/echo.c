#include <program/echo.h>
#include <console.h>
#include <syscall.h>

void echo(void) {
    struct key_event ke;

    while (1) {
        sys_console_read_key_event(&ke);
        sys_console_putdw_at(ke.scancode, 0, 0);
    }
}

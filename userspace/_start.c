#include <userspace.h>

extern char _bss_start[];
extern char _bss_end[];

void _start(void) {
    // TODO: Make bootloader clear BSS segment instead.
    char *bss = _bss_start;
    while (bss < _bss_end) {
        *bss++ = 0;
    }

    userspace_init();
}

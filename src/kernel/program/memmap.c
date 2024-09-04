#include <program/memmap.h>
#include <mm.h>

void memmap(void) {
    mm_show_mmap();
    while (1) {
        __asm__("hlt");
    }
}

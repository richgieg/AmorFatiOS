#include <program/counter.h>

void counter(void) {
    int count = 0;
    while (1) {
        // console_putdw_at(count, 0, 0);
        __asm__(
            "mov eax, %0;"
            "int 0x82;"
            :
            :
            "r" (count)
        );
        count++;
    }
}

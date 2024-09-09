#include <userspace.h>
#include <program/counter.h>

void userspace_init(void) {
    __asm__(
        "mov eax, 0xdeadbeef;"
        "int 0x82;"
    );
    __asm__(
        "mov eax, %0;"
        "int 0x81;"
        :
        :
        "r" (counter)
    );
    while (1) {
        // TODO: Do some userspace stuff here.
    }
}

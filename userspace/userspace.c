#include <userspace.h>

void userspace_init(void) {
    while (1) {
        __asm__("hlt");
    }
}

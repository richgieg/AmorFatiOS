#include "kernel.h"

void _start(void) {
    kernel_init();
    kernel_idle();
}

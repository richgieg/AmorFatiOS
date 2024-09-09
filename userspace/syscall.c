#include <syscall.h>
#include <types.h>

void sys_console_putdw_at(u32 dw, u8 col, u8 row) {
    __asm__(
        "mov eax, %0;"
        "int 0x82;"
        : : "r" (dw)
    );
}

void sys_console_read_key_event(struct key_event *ke) {
    __asm__(
        "mov eax, %0;"
        "int 0x83;"
        : : "r" (ke)
    );
}

void sys_process_create(void (*start)()) {
    __asm__(
        "mov eax, %0;"
        "int 0x81;"
        : : "r" (start)
    );
}

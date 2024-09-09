#include <sys.h>

void sys_process_create(void (*start)()) {
    __asm__(
        "mov eax, %0;"
        "mov ebx, %1;"
        "int 0x80;"
        :
        : "eax" (0), "ebx" (start)
    );
}

void sys_console_putdw_at(u32 dw, u8 col, u8 row) {
    __asm__(
        "mov eax, %0;"
        "mov ebx, %1;"
        "int 0x80;"
        :
        : "eax" (1), "ebx" (dw)
    );
}

void sys_console_read_key_event(struct key_event *ke) {
    __asm__(
        "mov eax, %0;"
        "mov ebx, %1;"
        "int 0x80;"
        :
        : "eax" (2), "ebx" (ke)
    );
}

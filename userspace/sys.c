#include <sys.h>

#define PROCESS_CREATE              0x0000
#define CONSOLE_PUTDW_AT            0x0001
#define CONSOLE_READ_KEY_EVENT      0x0002

void sys_process_create(void (*start)()) {
    __asm__(
        "mov eax, %0;"
        "mov ebx, %1;"
        "int 0x80;"
        :
        : "eax" (PROCESS_CREATE), "ebx" (start)
    );
}

void sys_console_putdw_at(u32 dw, u8 col, u8 row) {
    __asm__(
        "mov eax, %0;"
        "mov ebx, %1;"
        "int 0x80;"
        :
        : "eax" (CONSOLE_PUTDW_AT), "ebx" (dw)
    );
}

void sys_console_read_key_event(struct key_event *ke) {
    __asm__(
        "mov eax, %0;"
        "mov ebx, %1;"
        "int 0x80;"
        :
        : "eax" (CONSOLE_READ_KEY_EVENT), "ebx" (ke)
    );
}

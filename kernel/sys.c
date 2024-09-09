#include <sys.h>
#include <types.h>
#include <process.h>
#include <console.h>

#define PROCESS_CREATE              0x0000
#define CONSOLE_PUTDW_AT            0x0001
#define CONSOLE_READ_KEY_EVENT      0x0002

void sys_dispatch(void) {
    u32 eax;
    u32 ebx;
    u32 ecx;
    u32 edx;

    __asm__ volatile(
        "mov %0, eax;"
        "mov %1, ebx;"
        "mov %2, ecx;"
        "mov %3, edx;"
        : "=m" (eax), "=m" (ebx), "=m" (ecx), "=m" (edx)
        :
        : "memory"
    );

    switch (eax) {
        case PROCESS_CREATE:
            process_create((void (*)(void))ebx);
            break;
        case CONSOLE_PUTDW_AT:
            console_putdw_at(ebx, 0, 0);
            break;
        case CONSOLE_READ_KEY_EVENT:
            console_read_key_event((struct key_event *)ebx);
            break;
    }
}

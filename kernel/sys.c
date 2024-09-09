#include <sys.h>
#include <types.h>
#include <process.h>
#include <console.h>

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
        case 0:
            process_create((void (*)(void))ebx);
            break;
        case 1:
            console_putdw_at(ebx, 0, 0);
            break;
        case 2:
            console_read_key_event((struct key_event *)ebx);
            break;
    }
}

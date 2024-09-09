#include <sys.h>
#include <types.h>
#include <process.h>
#include <console.h>

#define PROCESS_CREATE              0x0000

#define CONSOLE_CLEAR               0x0001
#define CONSOLE_GET_BG_COLOR        0x0002
#define CONSOLE_SET_BG_COLOR        0x0003
#define CONSOLE_GET_TEXT_COLOR      0x0004
#define CONSOLE_SET_TEXT_COLOR      0x0005
#define CONSOLE_SET_POS             0x0006
#define CONSOLE_WRITEC              0x0007
#define CONSOLE_PUTC                0x0008
#define CONSOLE_PUTS                0x0009
#define CONSOLE_PUTB                0x000a
#define CONSOLE_PUTW                0x000b
#define CONSOLE_PUTDW               0x000c
// #define CONSOLE_PUTQW               0x000d
#define CONSOLE_PUTP                0x000e
#define CONSOLE_PUTC_AT             0x000f
#define CONSOLE_PUTS_AT             0x0010
#define CONSOLE_PUTB_AT             0x0011
#define CONSOLE_PUTW_AT             0x0012
#define CONSOLE_PUTDW_AT            0x0013
// #define CONSOLE_PUTQW_AT            0x0014
#define CONSOLE_PUTP_AT             0x0015
#define CONSOLE_READ_KEY_EVENT      0x0016
#define CONSOLE_GET_NUM_COLUMNS     0x0017
#define CONSOLE_GET_NUM_ROWS        0x0018

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
        case CONSOLE_CLEAR:
            console_clear();
            break;
        case CONSOLE_GET_BG_COLOR:
            break;
        case CONSOLE_SET_BG_COLOR:
            console_set_bg_color(ebx);
            break;
        case CONSOLE_GET_TEXT_COLOR:
            break;
        case CONSOLE_SET_TEXT_COLOR:
            console_set_text_color(ebx);
            break;
        case CONSOLE_SET_POS:
            break;
        case CONSOLE_WRITEC:
            console_writec(ebx);
            break;
        case CONSOLE_PUTC:
            console_putc(ebx);
            break;
        case CONSOLE_PUTS:
            console_puts((const char *)ebx);
            break;
        case CONSOLE_PUTB:
            console_putb(ebx);
            break;
        case CONSOLE_PUTW:
            console_putw(ebx);
            break;
        case CONSOLE_PUTDW:
            console_putdw(ebx);
            break;
        // case CONSOLE_PUTQW:
        //     console_putqw(ebx);
        //     break;
        case CONSOLE_PUTP:
            console_putp((void *)ebx);
            break;
        case CONSOLE_PUTC_AT:
            console_putc_at(ebx, ecx, edx);
            break;
        case CONSOLE_PUTS_AT:
            console_puts_at((const char *)ebx, ecx, edx);
            break;
        case CONSOLE_PUTB_AT:
            console_putb_at(ebx, ecx, edx);
            break;
        case CONSOLE_PUTW_AT:
            console_putw_at(ebx, ecx, edx);
            break;
        case CONSOLE_PUTDW_AT:
            console_putdw_at(ebx, ecx, edx);
            break;
        // case CONSOLE_PUTQW_AT:
        //     break;
        case CONSOLE_PUTP_AT:
            break;
        case CONSOLE_READ_KEY_EVENT:
            console_read_key_event((struct key_event *)ebx);
            break;
        case CONSOLE_GET_NUM_COLUMNS:
            break;
        case CONSOLE_GET_NUM_ROWS:
            break;
    }
}

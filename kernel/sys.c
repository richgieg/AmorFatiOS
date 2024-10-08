#include <sys.h>
#include <types.h>
#include <process.h>
#include <mm.h>
#include <console.h>
#include <net.h>

#define PROCESS_CREATE              0x0000
#define PROCESS_CREATE_EX           0x0001
#define PROCESS_EXIT                0x0002
#define PROCESS_WAIT_FOR_EXIT       0x0003
#define PROCESS_GET_CONSOLE_INDEX   0x0004
#define PROCESS_GET_CHILD_PIDS      0x0005

#define MM_ALLOC                    0x0006
#define MM_FREE                     0x0007
#define MM_GET_STATS                0x0008

#define CONSOLE_CLEAR               0x0009
#define CONSOLE_GET_BG_COLOR        0x000a
#define CONSOLE_SET_BG_COLOR        0x000b
#define CONSOLE_GET_TEXT_COLOR      0x000c
#define CONSOLE_SET_TEXT_COLOR      0x000d
#define CONSOLE_GET_COLUMN          0x000e
#define CONSOLE_GET_ROW             0x000f
#define CONSOLE_SET_POS             0x0010
#define CONSOLE_WRITEC              0x0011
#define CONSOLE_PUTC                0x0012
#define CONSOLE_PUTS                0x0013
#define CONSOLE_PUTB                0x0014
#define CONSOLE_PUTW                0x0015
#define CONSOLE_PUTDW               0x0016
#define CONSOLE_PUTQW               0x0017
#define CONSOLE_PUTP                0x0018
#define CONSOLE_PUTC_AT             0x0019
#define CONSOLE_PUTS_AT             0x001a
#define CONSOLE_PUTB_AT             0x001b
#define CONSOLE_PUTW_AT             0x001c
#define CONSOLE_PUTDW_AT            0x001d
#define CONSOLE_PUTQW_AT            0x001e
#define CONSOLE_PUTP_AT             0x001f
#define CONSOLE_READ_KEY_EVENT      0x0020
#define CONSOLE_GET_NUM_COLUMNS     0x0021
#define CONSOLE_GET_NUM_ROWS        0x0022

#define NET_SUBSCRIBE               0x0023
#define NET_UNSUBSCRIBE             0x0024
#define NET_READ_FRAME              0x0025

u32 sys_dispatch(void) {
    u32 result = 0;
    u32 syscall_num;
    u32 arg0;
    u32 arg1;
    u32 arg2;
    u32 arg3;

    __asm__(
        ""
        : "=a" (syscall_num),   // syscall_num = EAX
          "=b" (arg0),          // arg0 = EBX
          "=c" (arg1),          // arg1 = ECX
          "=d" (arg2),          // arg2 = EDX
          "=S" (arg3)           // arg3 = ESI
        : :
    );

    switch (syscall_num) {
        case PROCESS_CREATE:
            result = process_create((void (*)(void))arg0);
            break;
        case PROCESS_CREATE_EX:
            result = process_create_ex((void (*)(void))arg0, arg1);
            break;
        case PROCESS_EXIT:
            process_exit();
            break;
        case PROCESS_WAIT_FOR_EXIT:
            process_wait_for_exit(arg0);
            break;
        case PROCESS_GET_CONSOLE_INDEX:
            result = process_get_console_index();
            break;
        case PROCESS_GET_CHILD_PIDS:
            process_get_child_pids(arg0, (int *)arg1, arg2, (int*)arg3);
            break;
        case MM_ALLOC:
            result = (u32)mm_alloc(arg0);
            break;
        case MM_FREE:
            mm_free((void *)arg0);
            break;
        case MM_GET_STATS:
            mm_get_stats((struct mm_stats *)arg0);
            break;
        case CONSOLE_CLEAR:
            console_clear();
            break;
        case CONSOLE_GET_BG_COLOR:
            result = console_get_bg_color();
            break;
        case CONSOLE_SET_BG_COLOR:
            console_set_bg_color(arg0);
            break;
        case CONSOLE_GET_TEXT_COLOR:
            result = console_get_text_color();
            break;
        case CONSOLE_SET_TEXT_COLOR:
            console_set_text_color(arg0);
            break;
        case CONSOLE_GET_COLUMN:
            result = console_get_column();
            break;
        case CONSOLE_GET_ROW:
            result = console_get_row();
            break;
        case CONSOLE_SET_POS:
            console_set_pos(arg0, arg1);
            break;
        case CONSOLE_WRITEC:
            console_writec(arg0);
            break;
        case CONSOLE_PUTC:
            console_putc(arg0);
            break;
        case CONSOLE_PUTS:
            console_puts((const char *)arg0);
            break;
        case CONSOLE_PUTB:
            console_putb(arg0);
            break;
        case CONSOLE_PUTW:
            console_putw(arg0);
            break;
        case CONSOLE_PUTDW:
            console_putdw(arg0);
            break;
        case CONSOLE_PUTQW:
            console_putqw((u64)arg0 << 32 | arg1);
            break;
        case CONSOLE_PUTP:
            console_putp((void *)arg0);
            break;
        case CONSOLE_PUTC_AT:
            console_putc_at(arg0, arg1, arg2);
            break;
        case CONSOLE_PUTS_AT:
            console_puts_at((const char *)arg0, arg1, arg2);
            break;
        case CONSOLE_PUTB_AT:
            console_putb_at(arg0, arg1, arg2);
            break;
        case CONSOLE_PUTW_AT:
            console_putw_at(arg0, arg1, arg2);
            break;
        case CONSOLE_PUTDW_AT:
            console_putdw_at(arg0, arg1, arg2);
            break;
        case CONSOLE_PUTQW_AT:
            console_putqw_at((u64)arg0 << 32 | arg1, arg2, arg3);
            break;
        case CONSOLE_PUTP_AT:
            console_putp_at((void *)arg0, arg1, arg2);
            break;
        case CONSOLE_READ_KEY_EVENT:
            console_read_key_event((struct key_event *)arg0);
            break;
        case CONSOLE_GET_NUM_COLUMNS:
            result = console_get_num_columns();
            break;
        case CONSOLE_GET_NUM_ROWS:
            result = console_get_num_rows();
            break;
        case NET_SUBSCRIBE:
            net_subscribe();
            break;
        case NET_UNSUBSCRIBE:
            net_unsubscribe();
            break;
        case NET_READ_FRAME:
            result = net_read_frame((u8 *)arg0, arg1);
            break;
    }
    return result;
}

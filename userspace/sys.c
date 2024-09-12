#include <sys.h>

#define PROCESS_CREATE              0x0000
#define PROCESS_EXIT                0x0019

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
#define CONSOLE_PUTQW               0x000d
#define CONSOLE_PUTP                0x000e
#define CONSOLE_PUTC_AT             0x000f
#define CONSOLE_PUTS_AT             0x0010
#define CONSOLE_PUTB_AT             0x0011
#define CONSOLE_PUTW_AT             0x0012
#define CONSOLE_PUTDW_AT            0x0013
#define CONSOLE_PUTQW_AT            0x0014
#define CONSOLE_PUTP_AT             0x0015
#define CONSOLE_READ_KEY_EVENT      0x0016
#define CONSOLE_GET_NUM_COLUMNS     0x0017
#define CONSOLE_GET_NUM_ROWS        0x0018

int sys_process_create(void (*start)()) {
    int result;
    __asm__(
        "int 0x80"
        : "=a" (result)
        : "a" (PROCESS_CREATE),
          "b" (start)
        :
    );
    return result;
}

void sys_process_exit(void) {
    __asm__(
        "int 0x80"
        :
        : "a" (PROCESS_EXIT)
        :
    );
}

void sys_console_clear(void) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_CLEAR)
        :
    );
}

enum vga_color sys_console_get_bg_color(void) {
    int result;
    __asm__(
        "int 0x80"
        : "=a" (result)
        : "a" (CONSOLE_GET_BG_COLOR)
        :
    );
    return result;
}

void sys_console_set_bg_color(enum vga_color bg_color) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_SET_BG_COLOR),
          "b" (bg_color)
        :
    );
}

enum vga_color sys_console_get_text_color(void) {
    int result;
    __asm__(
        "int 0x80"
        : "=a" (result)
        : "a" (CONSOLE_GET_TEXT_COLOR)
        :
    );
    return result;
}

void sys_console_set_text_color(enum vga_color text_color) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_SET_TEXT_COLOR),
          "b" (text_color)
        :
    );
}

void sys_console_set_pos(u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_SET_POS),
          "b" (col),
          "c" (row)
        :
    );
}

void sys_console_writec(char c) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_WRITEC),
          "b" (c)
        :
    );
}

void sys_console_putc(char c) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTC),
          "b" (c)
        :
    );
}

void sys_console_puts(const char *str) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTS),
          "b" (str)
        :
    );
}

void sys_console_putb(u8 b) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTB),
          "b" (b)
        :
    );
}

void sys_console_putw(u16 w) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTW),
          "b" (w)
        :
    );
}

void sys_console_putdw(u32 dw) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTDW),
          "b" (dw)
        :
    );
}

void sys_console_putqw(u64 qw) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTQW),
          "b" ((u32)(qw >> 32)),
          "c" ((u32)qw)
        :
    );
}

void sys_console_putp(void *p) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTP),
          "b" (p)
        :
    );
}

void sys_console_putc_at(char c, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTC_AT),
          "b" (c),
          "c" (col),
          "d" (row)
        :
    );
}

void sys_console_puts_at(const char *str, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTS_AT),
          "b" (str),
          "c" (col),
          "d" (row)
        :
    );
}

void sys_console_putb_at(u8 b, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTB_AT),
          "b" (b),
          "c" (col),
          "d" (row)
        :
    );
}

void sys_console_putw_at(u16 w, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTW_AT),
          "b" (w),
          "c" (col),
          "d" (row)
        :
    );
}

void sys_console_putdw_at(u32 dw, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTDW_AT),
          "b" (dw),
          "c" (col),
          "d" (row)
        :
    );
}

void sys_console_putqw_at(u64 qw, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTQW_AT),
          "b" ((u32)(qw >> 32)),
          "c" ((u32)qw),
          "d" (col),
          "S" (row)
        :
    );
}

void sys_console_putp_at(void *p, u8 col, u8 row) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_PUTP_AT),
          "b" (p),
          "c" (col),
          "d" (row)
        :
    );
}

void sys_console_read_key_event(struct key_event *ke) {
    __asm__(
        "int 0x80"
        :
        : "a" (CONSOLE_READ_KEY_EVENT),
          "b" (ke)
        :
    );
}

int sys_console_get_num_columns(void) {
    int result;
    __asm__(
        "int 0x80"
        : "=a" (result)
        : "a" (CONSOLE_GET_NUM_COLUMNS)
        :
    );
    return result;
}

int sys_console_get_num_rows(void) {
    int result;
    __asm__(
        "int 0x80"
        : "=a" (result)
        : "a" (CONSOLE_GET_NUM_ROWS)
        :
    );
    return result;
}

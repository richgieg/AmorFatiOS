#ifndef CONSOLE_H
#define CONSOLE_H

#include "types.h"

#define CONSOLE_ROWS 25
#define CONSOLE_COLUMNS 80

enum console_color {
    CONSOLE_COLOR_BLACK,
    CONSOLE_COLOR_BLUE,
    CONSOLE_COLOR_GREEN,
    CONSOLE_COLOR_CYAN,
    CONSOLE_COLOR_RED,
    CONSOLE_COLOR_MAGENTA,
    CONSOLE_COLOR_BROWN,
    CONSOLE_COLOR_WHITE,
    CONSOLE_COLOR_GRAY,
    CONSOLE_COLOR_LIGHT_BLUE,
    CONSOLE_COLOR_LIGHT_GREEN,
    CONSOLE_COLOR_LIGHT_CYAN,
    CONSOLE_COLOR_LIGHT_RED,
    CONSOLE_COLOR_LIGHT_MAGENTA,
    CONSOLE_COLOR_YELLOW,
    CONSOLE_COLOR_BRIGHT_WHITE,
};

void console_init(void);
void console_clear(void);
void console_writec(char c);
void console_putc(char c);
void console_puts(const char *str);
void console_putb(u8);
void console_putw(u16 w);
void console_putdw(u32 dw);
void console_putqw(u64 qw);
void console_putp(void *p);
void console_putc_at(char c, u8 col, u8 row);
void console_puts_at(const char *str, u8 col, u8 row);
void console_putb_at(u8 b, u8 col, u8 row);
void console_putw_at(u16 w, u8 col, u8 row);
void console_putdw_at(u32 dw, u8 col, u8 row);
void console_putqw_at(u64 dw, u8 col, u8 row);
void console_putp_at(void *p, u8 col, u8 row);
void console_dump_chars(void);

#endif

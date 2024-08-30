#ifndef VGA_H
#define VGA_H

#include "types.h"

#define VGA_ROWS 25
#define VGA_COLUMNS 80

enum vga_color {
    VGA_COLOR_BLACK,
    VGA_COLOR_BLUE,
    VGA_COLOR_GREEN,
    VGA_COLOR_CYAN,
    VGA_COLOR_RED,
    VGA_COLOR_MAGENTA,
    VGA_COLOR_BROWN,
    VGA_COLOR_WHITE,
    VGA_COLOR_GRAY,
    VGA_COLOR_LIGHT_BLUE,
    VGA_COLOR_LIGHT_GREEN,
    VGA_COLOR_LIGHT_CYAN,
    VGA_COLOR_LIGHT_RED,
    VGA_COLOR_LIGHT_MAGENTA,
    VGA_COLOR_YELLOW,
    VGA_COLOR_BRIGHT_WHITE,
};

void vga_init(void);
void vga_clear(void);
enum vga_color vga_get_bg_color();
void vga_set_bg_color(enum vga_color bg_color);
enum vga_color vga_get_text_color();
void vga_set_text_color(enum vga_color text_color);
void vga_set_pos(u8 col, u8 row);
void vga_writec(char c);
void vga_putc(char c);
void vga_puts(const char *str);
void vga_putb(u8);
void vga_putw(u16 w);
void vga_putdw(u32 dw);
void vga_putqw(u64 qw);
void vga_putp(void *p);
void vga_putc_at(char c, u8 col, u8 row);
void vga_puts_at(const char *str, u8 col, u8 row);
void vga_putb_at(u8 b, u8 col, u8 row);
void vga_putw_at(u16 w, u8 col, u8 row);
void vga_putdw_at(u32 dw, u8 col, u8 row);
void vga_putqw_at(u64 dw, u8 col, u8 row);
void vga_putp_at(void *p, u8 col, u8 row);
void vga_dump_chars(void);

#endif

#ifndef VGA_H
#define VGA_H

#include <stdint.h>

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
void vga_putc_at(char c, uint8_t x, uint8_t y);
void vga_puts_at(const char *str, uint8_t x, uint8_t y);
void vga_dump_chars(void);

#endif

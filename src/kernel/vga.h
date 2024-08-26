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
enum vga_color vga_get_bg_color();
void vga_set_bg_color(enum vga_color color);
enum vga_color vga_get_text_color();
void vga_set_text_color(enum vga_color color);
void vga_putc(char c);
void vga_putb_at(uint8_t b, uint8_t x, uint8_t y);
void vga_putw_at(uint16_t w, uint8_t x, uint8_t y);
void vga_putdw_at(uint32_t dw, uint8_t x, uint8_t y);
void vga_putqw_at(uint64_t dw, uint8_t x, uint8_t y);
void vga_putc_at(char c, uint8_t x, uint8_t y);
void vga_puts_at(const char *str, uint8_t x, uint8_t y);
void vga_dump_chars(void);

#endif

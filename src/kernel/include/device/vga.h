#ifndef VGA_H
#define VGA_H

#include <types.h>

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
void vga_copy(void *src);

static inline u16 vga_entry(
    char character, enum vga_color bg_color, enum vga_color text_color
) {
    u8 attributes = bg_color << 4 | text_color;
    return (u8)character | (u16)attributes << 8;
}

#endif

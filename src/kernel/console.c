// Reference: https://en.wikipedia.org/wiki/VGA_text_mode

#include "console.h"
#include "port.h"

#define ROWS 25
#define COLUMNS 80

char *const screen = (char *)0xb8000;

enum console_color bg_color = CONSOLE_COLOR_CYAN;
enum console_color text_color = CONSOLE_COLOR_BRIGHT_WHITE;

void console_init(void) {
    // Enable usage of all 16 background colors.
    // This ensures that attribute bit 7 is used for color instead of blink.
    // https://www.reddit.com/r/osdev/comments/70fcig/blinking_text/

    // Read I/O Address 0x3da to reset index/data flip-flop
    inb(0x3da);
    // Write index 0x30 to 0x3c0 to set register index to 0x30
    outb(0x3c0, 0x30);
    // Read from 0x03c1 to get register contents
    uint8_t flags = inb(0x3c1);
    // Unset Bit 3 to disable Blink
    flags &= 0xf7;
    // Write to 0x3c0 to update register with changed value
    outb(0x3c0, flags);

    // Disable the cursor.
    outb(0x3d4, 0x0a);
    outb(0x3d5, 0x20);

    console_clear();
}

void console_clear(void) {
    uint8_t attributes = (bg_color << 4) | (text_color);
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        screen[i * 2] = ' ';
        screen[i * 2 + 1] = attributes;
    }
}

void console_print_at(const char * str, uint8_t x, uint8_t y) {
    console_print_at_color(str, x, y, bg_color, text_color);
}

void console_print_at_color(
    const char * str, uint8_t x, uint8_t y, enum console_color bg_color,
    enum console_color text_color
) {
    uint8_t attributes = (bg_color << 4) | (text_color);
    int pos = (y * COLUMNS * 2) + (x * 2);
    while (*str) {
        screen[pos++] = *str;
        screen[pos++] = attributes;
        str++;
    }
}

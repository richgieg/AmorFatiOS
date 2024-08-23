// Reference: https://en.wikipedia.org/wiki/VGA_text_mode

#include "console.h"

#define ROWS 25
#define COLUMNS 80

char *const screen = (char *)0xb8000;

void console_init(void) {
    // Enable usage of all 16 background colors.
    // This ensures that attribute bit 7 is used for color instead of blink.
    // https://www.reddit.com/r/osdev/comments/70fcig/blinking_text/
    __asm__(
        // Read I/O Address 0x03da to reset index/data flip-flop
        "mov dx, 0x03da\n"
        "in al, dx\n"
        // Write index 0x30 to 0x03c0 to set register index to 0x30
        "mov dx, 0x03c0\n"
        "mov al, 0x30\n"
        "out dx, al\n"
        // Read from 0x03c1 to get register contents
        "inc dx\n"
        "in al, dx\n"
        // Unset Bit 3 to disable Blink
        "and al, 0xf7\n"
        // Write to 0x03c0 to update register with changed value
        "dec dx\n"
        "out dx, al\n"
    );
}

void console_set_bg_color(enum console_color color) {
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        char attributes = screen[i * 2 + 1];
        screen[i * 2 + 1] = (attributes & 0x0f) | (color << 4);
    }
}

void console_set_text_color(uint8_t color) {
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        char attributes = screen[i * 2 + 1];
        screen[i * 2 + 1] = (attributes & 0xf0) | (color & 0x0f);
    }
}

void console_print_at(const char * str, uint8_t x, uint8_t y) {
    int pos = (y * COLUMNS * 2) + (x * 2);
    while (*str) {
        screen[pos] = *str;
        pos += 2;
        str++;
    }
}

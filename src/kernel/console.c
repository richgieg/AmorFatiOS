#include "console.h"

#define ROWS 25
#define COLUMNS 80

char *const screen = (char *)0xb8000;

void console_set_bg_color(uint8_t color) {
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

void console_print_at(uint8_t row, uint8_t col, const char * str) {
    int pos = (row * COLUMNS * 2) + (col * 2);
    while (*str) {
        screen[pos] = *str;
        pos += 2;
        str++;
    }
}

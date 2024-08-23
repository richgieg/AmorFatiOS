#include <stdint.h>

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
void console_set_bg_color(enum console_color color);
void console_set_text_color(uint8_t color);
void console_print_at(const char * str, uint8_t x, uint8_t y);

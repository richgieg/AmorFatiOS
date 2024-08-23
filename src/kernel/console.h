#include <stdint.h>

void console_set_bg_color(uint8_t color);
void console_set_text_color(uint8_t color);
void console_print_at(const char * str, uint8_t x, uint8_t y);

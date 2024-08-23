#include <stdint.h>

void console_set_bg_color(uint8_t color);
void console_set_text_color(uint8_t color);
void console_print_at(uint8_t row, uint8_t col, const char * str);

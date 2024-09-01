#include "bugcheck.h"
#include "console.h"

void bugcheck(const char *file, int line, const char *message) {
    __asm__("cli");
    console_set_bg_color(CONSOLE_COLOR_RED);
    console_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_clear();
    console_puts("*** BUGCHECK ***\n\n");
    console_puts(message);
    console_puts("\n\nFile: ");
    console_puts(file);
    console_puts("\nLine: ");
    console_putdw(line);
    __asm__("hlt");
}

void exception(u8 vector) {
    __asm__("cli");
    console_set_bg_color(CONSOLE_COLOR_RED);
    console_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_clear();
    console_puts("*** EXCEPTION ***\n\n");
    console_puts("Vector: 0x");
    console_putb(vector);
    __asm__("hlt");
}

void exception_with_code(u8 vector, u32 code) {
    __asm__("cli");
    console_set_bg_color(CONSOLE_COLOR_RED);
    console_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_clear();
    console_puts("*** EXCEPTION ***\n\n");
    console_puts("Vector: 0x");
    console_putb(vector);
    console_puts("\nError Code: 0x");
    console_putdw(code);
    __asm__("hlt");
}

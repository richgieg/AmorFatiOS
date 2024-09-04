#include <bugcheck.h>
#include <console.h>

void bugcheck(const char *file, int line, const char *message) {
    __asm__("cli");
    console_dbg_set_bg_color(CONSOLE_COLOR_RED);
    console_dbg_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_dbg_clear();
    console_dbg_puts("*** BUGCHECK ***\n\n");
    console_dbg_puts(message);
    console_dbg_puts("\n\nFile: ");
    console_dbg_puts(file);
    console_dbg_puts("\nLine: ");
    console_dbg_putdw(line);
    console_show_dbg();
    __asm__("hlt");
}

void exception(u8 vector) {
    __asm__("cli");
    console_dbg_set_bg_color(CONSOLE_COLOR_RED);
    console_dbg_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_dbg_clear();
    console_dbg_puts("*** EXCEPTION ***\n\n");
    console_dbg_puts("Vector: 0x");
    console_dbg_putb(vector);
    console_show_dbg();
    __asm__("hlt");
}

void exception_with_code(u8 vector, u32 code) {
    __asm__("cli");
    console_dbg_set_bg_color(CONSOLE_COLOR_RED);
    console_dbg_set_text_color(CONSOLE_COLOR_BRIGHT_WHITE);
    console_dbg_clear();
    console_dbg_puts("*** EXCEPTION ***\n\n");
    console_dbg_puts("Vector: 0x");
    console_dbg_putb(vector);
    console_dbg_puts("\nError Code: 0x");
    console_dbg_putdw(code);
    console_show_dbg();
    __asm__("hlt");
}

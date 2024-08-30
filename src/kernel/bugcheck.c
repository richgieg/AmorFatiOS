#include "bugcheck.h"
#include "vga.h"

void bugcheck(const char *file, int line, const char *message) {
    __asm__("cli");
    vga_set_bg_color(VGA_COLOR_RED);
    vga_set_text_color(VGA_COLOR_BRIGHT_WHITE);
    vga_clear();
    vga_puts("*** BUGCHECK ***\n\n");
    vga_puts(message);
    vga_puts("\n\nFile: ");
    vga_puts(file);
    vga_puts("\nLine: ");
    vga_putdw(line);
    __asm__("hlt");
}

void exception(u8 vector) {
    __asm__("cli");
    vga_set_bg_color(VGA_COLOR_RED);
    vga_set_text_color(VGA_COLOR_BRIGHT_WHITE);
    vga_clear();
    vga_puts("*** EXCEPTION ***\n\n");
    vga_puts("Vector: 0x");
    vga_putb(vector);
    __asm__("hlt");
}

void exception_with_code(u8 vector, u32 code) {
    __asm__("cli");
    vga_set_bg_color(VGA_COLOR_RED);
    vga_set_text_color(VGA_COLOR_BRIGHT_WHITE);
    vga_clear();
    vga_puts("*** EXCEPTION ***\n\n");
    vga_puts("Vector: 0x");
    vga_putb(vector);
    vga_puts("\nError Code: 0x");
    vga_putdw(code);
    __asm__("hlt");
}

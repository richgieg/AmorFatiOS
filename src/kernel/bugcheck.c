#include "bugcheck.h"
#include "vga.h"

void bugcheck(const char *file, int line, const char *message) {
    vga_set_bg_color(VGA_COLOR_RED);
    vga_set_text_color(VGA_COLOR_BRIGHT_WHITE);
    vga_clear();
    vga_puts("*** BUGCHECK ***\n\n");
    vga_puts(message);
    vga_puts("\n\nFile: ");
    vga_puts(file);
    vga_puts("\nLine: ");
    vga_putdw(line);

    __asm__(
        "cli\n"
        "hlt\n"
    );
}

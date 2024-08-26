#include "memdump.h"
#include "vga.h"
#include <stdint.h>

static volatile uint8_t *memory_ptr = (volatile uint8_t *)0;

void md_next(void) {
    for (int k = 0; k < 25; k++) {
        vga_putdw((uint32_t)memory_ptr);
        vga_putc(' ');
        vga_putc(' ');
        vga_putc(' ');
        vga_putc(' ');

        for (int i = 0; i < 16; i++) {
            vga_putb(*memory_ptr);
            vga_putc(' ');
            memory_ptr++;
        }

        vga_putc(' ');
        vga_putc(' ');
        vga_putc(' ');

        memory_ptr -= 16;
        for (int i = 0; i < 16; i++) {
            vga_writec(*memory_ptr);
            memory_ptr++;
        }
        if (k < 24) {
            vga_putc('\n');
        }
    }
}

void md_prev(void) {
    memory_ptr -= 16 * 25;
    md_next();
}

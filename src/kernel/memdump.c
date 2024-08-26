#include "memdump.h"
#include "vga.h"
#include <stdint.h>

#define BYTES_PER_LINE 16
#define BYTES_PER_PAGE 4096
#define BYTES_PER_MB 1024 * 1024

static uint32_t cur_addr = 0x00000000;

void md_show(void) {
    volatile uint8_t *memory_ptr = (volatile uint8_t *)cur_addr;
    vga_set_pos(0, 0);

    for (int k = 0; k < VGA_ROWS; k++) {
        vga_putdw((uint32_t)memory_ptr);
        vga_puts("    ");

        for (int i = 0; i < BYTES_PER_LINE; i++) {
            vga_putb(*memory_ptr);
            vga_putc(' ');
            memory_ptr++;
        }

        vga_puts("   ");
        memory_ptr -= BYTES_PER_LINE;

        for (int i = 0; i < BYTES_PER_LINE; i++) {
            vga_writec(*memory_ptr);
            memory_ptr++;
        }

        if (k < VGA_ROWS - 1) {
            vga_putc('\n');
        }
    }
}

void md_next_line(void) {
    cur_addr += BYTES_PER_LINE;
    md_show();
}

void md_prev_line(void) {
    cur_addr -= BYTES_PER_LINE;
    md_show();
}

void md_next_page(void) {
    cur_addr += BYTES_PER_PAGE;
    md_show();
}

void md_prev_page(void) {
    cur_addr -= BYTES_PER_PAGE;
    md_show();
}

void md_next_mb(void) {
    cur_addr += BYTES_PER_MB;
    md_show();
}

void md_prev_mb(void) {
    cur_addr -= BYTES_PER_MB;
    md_show();
}

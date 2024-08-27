#include "mm.h"
#include "vga.h"
#include <stdint.h>

#define MMAP_ADDRESS 0x10000
#define BYTES_PER_LINE 16
#define BYTES_PER_PAGE 4096
#define BYTES_PER_MB 1024 * 1024

typedef struct {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
    uint32_t extended_attributes;
} __attribute__((packed)) mmap_entry;

static uint32_t cur_addr = 0x00000000;

void mm_init(void) {

}

void mm_show_mmap(void) {
    mmap_entry *e = (mmap_entry *)MMAP_ADDRESS;

    while (e->base_address || e->length || e->type || e->extended_attributes) {
        vga_putqw(e->base_address);
        vga_putc(' ');
        vga_putqw(e->length);
        vga_putc(' ');
        vga_putdw(e->type);
        vga_putc(' ');
        vga_putdw(e->extended_attributes);
        vga_putc('\n');
        e++;
    }
}

void mm_md_show(void) {
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

void mm_md_next_line(void) {
    cur_addr += BYTES_PER_LINE;
    mm_md_show();
}

void mm_md_prev_line(void) {
    cur_addr -= BYTES_PER_LINE;
    mm_md_show();
}

void mm_md_next_page(void) {
    cur_addr += BYTES_PER_PAGE;
    mm_md_show();
}

void mm_md_prev_page(void) {
    cur_addr -= BYTES_PER_PAGE;
    mm_md_show();
}

void mm_md_next_mb(void) {
    cur_addr += BYTES_PER_MB;
    mm_md_show();
}

void mm_md_prev_mb(void) {
    cur_addr -= BYTES_PER_MB;
    mm_md_show();
}

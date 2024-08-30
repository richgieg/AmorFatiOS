#include "mm.h"
#include "vga.h"
#include "types.h"

#define MMAP_ADDRESS 0x10000
#define MDUMP_BYTES_PER_LINE 16
#define MDUMP_BYTES_PER_PAGE 4096
#define MDUMP_BYTES_PER_MB 1024 * 1024

struct mmap_entry {
    u64 base_address;
    u64 length;
    u32 type;
    u32 extended_attributes;
} __attribute__((packed)) mmap_entry;

static u32 mdump_cur_addr = 0x00000000;

void mm_init(void) {

}

void mm_show_mmap(void) {
    struct mmap_entry *e = (struct mmap_entry *)MMAP_ADDRESS;

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

void mm_show_mdump(void) {
    volatile u8 *memory_ptr = (volatile u8 *)mdump_cur_addr;
    vga_set_pos(0, 0);

    for (int k = 0; k < VGA_ROWS; k++) {
        vga_putdw((u32)memory_ptr);
        vga_puts("    ");

        for (int i = 0; i < MDUMP_BYTES_PER_LINE; i++) {
            vga_putb(*memory_ptr);
            vga_putc(' ');
            memory_ptr++;
        }

        vga_puts("   ");
        memory_ptr -= MDUMP_BYTES_PER_LINE;

        for (int i = 0; i < MDUMP_BYTES_PER_LINE; i++) {
            vga_writec(*memory_ptr);
            memory_ptr++;
        }

        if (k < VGA_ROWS - 1) {
            vga_putc('\n');
        }
    }
}

void mm_mdump_next_byte(void) {
    mdump_cur_addr += 1;
    mm_show_mdump();
}

void mm_mdump_prev_byte(void) {
    mdump_cur_addr -= 1;
    mm_show_mdump();
}

void mm_mdump_next_line(void) {
    mdump_cur_addr += MDUMP_BYTES_PER_LINE;
    mm_show_mdump();
}

void mm_mdump_prev_line(void) {
    mdump_cur_addr -= MDUMP_BYTES_PER_LINE;
    mm_show_mdump();
}

void mm_mdump_next_page(void) {
    mdump_cur_addr += MDUMP_BYTES_PER_PAGE;
    mm_show_mdump();
}

void mm_mdump_prev_page(void) {
    mdump_cur_addr -= MDUMP_BYTES_PER_PAGE;
    mm_show_mdump();
}

void mm_mdump_next_mb(void) {
    mdump_cur_addr += MDUMP_BYTES_PER_MB;
    mm_show_mdump();
}

void mm_mdump_prev_mb(void) {
    mdump_cur_addr -= MDUMP_BYTES_PER_MB;
    mm_show_mdump();
}

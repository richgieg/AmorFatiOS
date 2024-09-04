#include <mm.h>
#include <console.h>
#include <types.h>

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
        console_putqw(e->base_address);
        console_putc(' ');
        console_putqw(e->length);
        console_putc(' ');
        console_putdw(e->type);
        console_putc(' ');
        console_putdw(e->extended_attributes);
        console_putc('\n');
        e++;
    }
}

void mm_show_mdump(void) {
    volatile u8 *memory_ptr = (volatile u8 *)mdump_cur_addr;
    console_set_pos(0, 0);

    for (int k = 0; k < CONSOLE_ROWS; k++) {
        console_putdw((u32)memory_ptr);
        console_puts("    ");

        for (int i = 0; i < MDUMP_BYTES_PER_LINE; i++) {
            console_putb(*memory_ptr);
            console_putc(' ');
            memory_ptr++;
        }

        console_puts("   ");
        memory_ptr -= MDUMP_BYTES_PER_LINE;

        for (int i = 0; i < MDUMP_BYTES_PER_LINE; i++) {
            console_writec(*memory_ptr);
            memory_ptr++;
        }

        if (k < CONSOLE_ROWS - 1) {
            console_putc('\n');
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

#include "mm.h"
#include "vga.h"
#include <stdint.h>

#define MMAP_ADDRESS 0x10000

typedef struct {
    uint64_t base_address;
    uint64_t length;
    uint32_t type;
    uint32_t extended_attributes;
} __attribute__((packed)) mmap_entry;

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

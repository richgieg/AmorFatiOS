#include <program/memmap.h>
#include <types.h>
#include <console.h>

// Memory map is put here by the bootloader.
#define MEMMAP_ADDRESS 0x10000

struct memmap_entry {
    u64 base_address;
    u64 length;
    u32 type;
    u32 extended_attributes;
} __attribute__((packed));

static void show_memmap(void);

void memmap(void) {
    show_memmap();
    while (1) {
        __asm__("hlt");
    }
}

static void show_memmap(void) {
    struct memmap_entry *e = (struct memmap_entry *)MEMMAP_ADDRESS;

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

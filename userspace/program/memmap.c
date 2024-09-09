#include <program/memmap.h>
#include <types.h>
#include <sys.h>

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
    while (true);
}

static void show_memmap(void) {
    struct memmap_entry *e = (struct memmap_entry *)MEMMAP_ADDRESS;

    while (e->base_address || e->length || e->type || e->extended_attributes) {
        sys_console_putqw(e->base_address);
        sys_console_putc(' ');
        sys_console_putqw(e->length);
        sys_console_putc(' ');
        sys_console_putdw(e->type);
        sys_console_putc(' ');
        sys_console_putdw(e->extended_attributes);
        sys_console_putc('\n');
        e++;
    }
}

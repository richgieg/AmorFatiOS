#include <tss.h>
#include <gdt.h>

struct tss_entry {
	u32 prev_tss; // The previous TSS - with hardware task switching these form a kind of backward linked list.
	u32 esp0;     // The stack pointer to load when changing to kernel mode.
	u32 ss0;      // The stack segment to load when changing to kernel mode.
	// Everything below here is unused.
	u32 esp1; // esp and ss 1 and 2 would be used when switching to rings 1 or 2.
	u32 ss1;
	u32 esp2;
	u32 ss2;
	u32 cr3;
	u32 eip;
	u32 eflags;
	u32 eax;
	u32 ecx;
	u32 edx;
	u32 ebx;
	u32 esp;
	u32 ebp;
	u32 esi;
	u32 edi;
	u32 es;
	u32 cs;
	u32 ss;
	u32 ds;
	u32 fs;
	u32 gs;
	u32 ldt;
	u16 trap;
	u16 iomap_base;
} __attribute__((packed));

static struct tss_entry tss_entry;

void tss_init(void) {
    u32 base = (u32)&tss_entry;
    u32 limit = sizeof(tss_entry) - 1;

    struct gdt_entry *gdt_tss_entry = gdt_get_tss_entry();
    gdt_tss_entry->limit_low = limit;
    gdt_tss_entry->base_low = base;
    gdt_tss_entry->accessed = 1;
    gdt_tss_entry->read_write = 0;
    gdt_tss_entry->conforming_expand_down = 0;
    gdt_tss_entry->code = 1;
    gdt_tss_entry->code_data_segment = 0;
    gdt_tss_entry->DPL = 0;
    gdt_tss_entry->present = 1;
    gdt_tss_entry->limit_high = limit >> 16;
    gdt_tss_entry->available = 0;
    gdt_tss_entry->long_mode = 0;
    gdt_tss_entry->big = 0;
    gdt_tss_entry->gran = 0;
    gdt_tss_entry->base_high = base >> 24;

    tss_entry.ss0 = 0x10;
}

void tss_set_kernel_stack(u32 esp) {
    tss_entry.esp0 = esp;
}

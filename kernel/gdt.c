#include <gdt.h>
#include <types.h>

struct gdtr {
    u16 limit;
    u32 base;
} __attribute__((packed));

static struct gdtr gdtr;

// Null segment, kernel code + data, user code + data, TSS.
static struct gdt_entry gdt[6];

void gdt_init(void) {
    gdtr.base = (u32)&gdt[0];
    gdtr.limit = sizeof(gdt) - 1;

    struct gdt_entry *kernel_code = &gdt[1];
    struct gdt_entry *kernel_data = &gdt[2];
    struct gdt_entry *user_code = &gdt[3];
    struct gdt_entry *user_data = &gdt[4];

    kernel_code->limit_low = 0xffff;
    kernel_code->base_low = 0;
    kernel_code->accessed = 0;
    kernel_code->read_write = 1;
    kernel_code->conforming_expand_down = 0;
    kernel_code->code = 1;
    kernel_code->code_data_segment = 1;
    kernel_code->DPL = 0;
    kernel_code->present = 1;
    kernel_code->limit_high = 0xf;
    kernel_code->available = 1;
    kernel_code->long_mode = 0;
    kernel_code->big = 1;
    kernel_code->gran = 1;
    kernel_code->base_high = 0;

    *kernel_data = *kernel_code;
    kernel_data->code = 0;

    *user_code = *kernel_code;
    user_code->DPL = 3;

    *user_data = *user_code;
    user_data->code = 0;

    __asm__("lgdt %0" : : "m"(gdtr));
}

struct gdt_entry * gdt_get_tss_entry(void) {
    return &gdt[5];
}

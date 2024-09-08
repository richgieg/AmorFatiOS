#ifndef GDT_H
#define GDT_H

struct gdt_entry {
    unsigned int limit_low              : 16;
    unsigned int base_low               : 24;
    unsigned int accessed               :  1;
    unsigned int read_write             :  1;
    unsigned int conforming_expand_down :  1;
    unsigned int code                   :  1;
    unsigned int code_data_segment      :  1;
    unsigned int DPL                    :  2;
    unsigned int present                :  1;
    unsigned int limit_high             :  4;
    unsigned int available              :  1;
    unsigned int long_mode              :  1;
    unsigned int big                    :  1;
    unsigned int gran                   :  1;
    unsigned int base_high              :  8;
} __attribute__((packed));

void gdt_init(void);
struct gdt_entry * gdt_get_tss_entry(void);

#endif

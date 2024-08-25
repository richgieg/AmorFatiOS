#include "idt.h"
#include "vga.h"

#define IDT_MAX_DESCRIPTORS 256

typedef struct {
    uint16_t isr_low; // lower 16 bits of ISR's address
    uint16_t kernel_cs; // code segment selector
    uint8_t reserved; // should be set to zero
    uint8_t attributes; // type and attributes
    uint16_t isr_high; // higher 16 bites of ISR's address
} __attribute__((packed)) idt_entry_t;

__attribute__((aligned(0x10)))
static idt_entry_t idt[256]; // array of IDT entries aligned for performance

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idtr_t;

static idtr_t idtr;

void idt_set_descriptor(uint8_t vector, void *isr, uint8_t flags) {
    idt_entry_t *descriptor = &idt[vector];
    descriptor->isr_low = (uint32_t)isr & 0xffff;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (uint32_t)isr >> 16;
    descriptor->reserved = 0;
}

__attribute__((naked))
void exception_handler_00(void) {
    vga_print_at("00", 0, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_01(void) {
    vga_print_at("01", 2, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_02(void) {
    vga_print_at("02", 4, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_03(void) {
    vga_print_at("03", 6, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_04(void) {
    vga_print_at("04", 8, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_05(void) {
    vga_print_at("05", 10, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_06(void) {
    vga_print_at("06", 12, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_07(void) {
    vga_print_at("07", 14, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_08(void) {
    vga_print_at("08", 16, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_09(void) {
    vga_print_at("09", 18, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_10(void) {
    vga_print_at("10", 20, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_11(void) {
    vga_print_at("11", 22, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_12(void) {
    vga_print_at("12", 24, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_13(void) {
    vga_print_at("13", 26, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_14(void) {
    vga_print_at("14", 28, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_15(void) {
    vga_print_at("15", 30, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_16(void) {
    vga_print_at("16", 32, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_17(void) {
    vga_print_at("17", 34, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_18(void) {
    vga_print_at("18", 36, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_19(void) {
    vga_print_at("19", 38, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_20(void) {
    vga_print_at("20", 40, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_21(void) {
    vga_print_at("21", 42, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_22(void) {
    vga_print_at("22", 44, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_23(void) {
    vga_print_at("23", 46, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_24(void) {
    vga_print_at("24", 48, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_25(void) {
    vga_print_at("25", 50, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_26(void) {
    vga_print_at("26", 52, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_27(void) {
    vga_print_at("27", 54, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_28(void) {
    vga_print_at("28", 56, 1);
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_29(void) {
    vga_print_at("29", 58, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_30(void) {
    vga_print_at("30", 60, 1);
    __asm__("add esp, 4"); // remove error code from stack
    __asm__("iret");
}

__attribute__((naked))
void exception_handler_31(void) {
    vga_print_at("31", 62, 1);
    __asm__("iret");
}

void idt_init(void) {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (uint16_t)sizeof(idt_entry_t) * IDT_MAX_DESCRIPTORS - 1;

    idt_set_descriptor(0, exception_handler_00, 0x8e);
    idt_set_descriptor(1, exception_handler_01, 0x8e);
    idt_set_descriptor(2, exception_handler_02, 0x8e);
    idt_set_descriptor(3, exception_handler_03, 0x8e);
    idt_set_descriptor(4, exception_handler_04, 0x8e);
    idt_set_descriptor(5, exception_handler_05, 0x8e);
    idt_set_descriptor(6, exception_handler_06, 0x8e);
    idt_set_descriptor(7, exception_handler_07, 0x8e);
    idt_set_descriptor(8, exception_handler_08, 0x8e);
    idt_set_descriptor(9, exception_handler_09, 0x8e);
    idt_set_descriptor(10, exception_handler_10, 0x8e);
    idt_set_descriptor(11, exception_handler_11, 0x8e);
    idt_set_descriptor(12, exception_handler_12, 0x8e);
    idt_set_descriptor(13, exception_handler_13, 0x8e);
    idt_set_descriptor(14, exception_handler_14, 0x8e);
    idt_set_descriptor(15, exception_handler_15, 0x8e);
    idt_set_descriptor(16, exception_handler_16, 0x8e);
    idt_set_descriptor(17, exception_handler_17, 0x8e);
    idt_set_descriptor(18, exception_handler_18, 0x8e);
    idt_set_descriptor(19, exception_handler_19, 0x8e);
    idt_set_descriptor(20, exception_handler_20, 0x8e);
    idt_set_descriptor(21, exception_handler_21, 0x8e);
    idt_set_descriptor(22, exception_handler_22, 0x8e);
    idt_set_descriptor(23, exception_handler_23, 0x8e);
    idt_set_descriptor(24, exception_handler_24, 0x8e);
    idt_set_descriptor(25, exception_handler_25, 0x8e);
    idt_set_descriptor(26, exception_handler_26, 0x8e);
    idt_set_descriptor(27, exception_handler_27, 0x8e);
    idt_set_descriptor(28, exception_handler_28, 0x8e);
    idt_set_descriptor(29, exception_handler_29, 0x8e);
    idt_set_descriptor(30, exception_handler_30, 0x8e);
    idt_set_descriptor(31, exception_handler_31, 0x8e);

    __asm__("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__("sti"); // enable interrupts
}

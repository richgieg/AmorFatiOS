#include "idt.h"
#include "vga.h"
#include "bugcheck.h"

#define IDT_MAX_DESCRIPTORS 256

struct idtr {
    u16 limit;
    u32 base;
} __attribute__((packed));

struct idt_entry {
    u16 isr_low; // lower 16 bits of ISR's address
    u16 kernel_cs; // code segment selector
    u8 reserved; // should be set to zero
    u8 attributes; // type and attributes
    u16 isr_high; // higher 16 bites of ISR's address
} __attribute__((packed));

static struct idtr idtr;

__attribute__((aligned(0x10)))
static struct idt_entry idt[256]; // array of IDT entries aligned for performance

void idt_set_descriptor(u8 vector, void *isr, u8 flags) {
    struct idt_entry *descriptor = &idt[vector];
    descriptor->isr_low = (u32)isr & 0xffff;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (u32)isr >> 16;
    descriptor->reserved = 0;
}

__attribute__((naked))
void exception_handler_00(void) {
    exception(0);
}

__attribute__((naked))
void exception_handler_01(void) {
    exception(1);
}

__attribute__((naked))
void exception_handler_02(void) {
    exception(2);
}

__attribute__((naked))
void exception_handler_03(void) {
    exception(3);
}

__attribute__((naked))
void exception_handler_04(void) {
    exception(4);
}

__attribute__((naked))
void exception_handler_05(void) {
    exception(5);
}

__attribute__((naked))
void exception_handler_06(void) {
    exception(6);
}

__attribute__((naked))
void exception_handler_07(void) {
    exception(7);
}

__attribute__((naked))
void exception_handler_08(void) {
    __asm__(
        "push 8\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_09(void) {
    exception(9);
}

__attribute__((naked))
void exception_handler_10(void) {
    __asm__(
        "push 10\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_11(void) {
    __asm__(
        "push 11\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_12(void) {
    __asm__(
        "push 12\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_13(void) {
    __asm__(
        "push 13\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_14(void) {
    __asm__(
        "push 14\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_16(void) {
    exception(16);
}

__attribute__((naked))
void exception_handler_17(void) {
    __asm__(
        "push 17\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
void exception_handler_18(void) {
    exception(18);
}

__attribute__((naked))
void exception_handler_19(void) {
    exception(19);
}

__attribute__((naked))
void return_from_interrupt(void) {
    __asm__("iret");
}

void idt_init(void) {
    idtr.base = (uintptr_t)&idt[0];
    idtr.limit = (u16)sizeof(struct idt_entry) * IDT_MAX_DESCRIPTORS - 1;

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
    idt_set_descriptor(16, exception_handler_16, 0x8e);
    idt_set_descriptor(17, exception_handler_17, 0x8e);
    idt_set_descriptor(18, exception_handler_18, 0x8e);
    idt_set_descriptor(19, exception_handler_19, 0x8e);

    __asm__("lidt %0" : : "m"(idtr)); // load the new IDT
    __asm__("sti"); // enable interrupts
}

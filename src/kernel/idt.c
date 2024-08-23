#include <stdint.h>
#include "pic.h"
#include "console.h"

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
    console_print_at(1, 0, "00");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_01(void) {
    console_print_at(1, 2, "01");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_02(void) {
    console_print_at(1, 4, "02");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_03(void) {
    console_print_at(1, 6, "03");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_04(void) {
    console_print_at(1, 8, "04");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_05(void) {
    console_print_at(1, 10, "05");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_06(void) {
    console_print_at(1, 12, "06");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_07(void) {
    console_print_at(1, 14, "07");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_08(void) {
    console_print_at(1, 16, "08");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_09(void) {
    console_print_at(1, 18, "09");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_10(void) {
    console_print_at(1, 20, "10");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_11(void) {
    console_print_at(1, 22, "11");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_12(void) {
    console_print_at(1, 24, "12");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_13(void) {
    console_print_at(1, 26, "13");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_14(void) {
    console_print_at(1, 28, "14");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_15(void) {
    console_print_at(1, 30, "15");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_16(void) {
    console_print_at(1, 32, "16");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_17(void) {
    console_print_at(1, 34, "17");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_18(void) {
    console_print_at(1, 36, "18");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_19(void) {
    console_print_at(1, 38, "19");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_20(void) {
    console_print_at(1, 40, "20");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_21(void) {
    console_print_at(1, 42, "21");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_22(void) {
    console_print_at(1, 44, "22");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_23(void) {
    console_print_at(1, 46, "23");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_24(void) {
    console_print_at(1, 48, "24");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_25(void) {
    console_print_at(1, 50, "25");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_26(void) {
    console_print_at(1, 52, "26");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_27(void) {
    console_print_at(1, 54, "27");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_28(void) {
    console_print_at(1, 56, "28");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_29(void) {
    console_print_at(1, 58, "29");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_30(void) {
    console_print_at(1, 60, "30");
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_31(void) {
    console_print_at(1, 62, "31");
    __asm__ volatile ("iret");
}

__attribute__((naked))
void irq_handler_01(void) {
    console_print_at(0, 79, "#");
    __asm__ volatile ("iret");
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
    idt_set_descriptor(IRQ1_INTERRUPT, irq_handler_01, 0x8e);

    __asm__ volatile ("lidt %0" : : "m"(idtr)); // load the new IDT
}

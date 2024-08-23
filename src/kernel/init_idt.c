#include <stdint.h>
#include "pic.h"

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
    char *p = (char *)0xb8000;
    p[160] = '0';
    p[162] = '0';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_01(void) {
    char *p = (char *)0xb8000;
    p[164] = '0';
    p[166] = '1';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_02(void) {
    char *p = (char *)0xb8000;
    p[168] = '0';
    p[170] = '2';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_03(void) {
    char *p = (char *)0xb8000;
    p[172] = '0';
    p[174] = '3';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_04(void) {
    char *p = (char *)0xb8000;
    p[176] = '0';
    p[178] = '4';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_05(void) {
    char *p = (char *)0xb8000;
    p[180] = '0';
    p[182] = '5';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_06(void) {
    char *p = (char *)0xb8000;
    p[184] = '0';
    p[186] = '6';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_07(void) {
    char *p = (char *)0xb8000;
    p[188] = '0';
    p[190] = '7';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_08(void) {
    char *p = (char *)0xb8000;
    p[192] = '0';
    p[194] = '8';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_09(void) {
    char *p = (char *)0xb8000;
    p[196] = '0';
    p[198] = '9';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_10(void) {
    char *p = (char *)0xb8000;
    p[200] = '1';
    p[202] = '0';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_11(void) {
    char *p = (char *)0xb8000;
    p[204] = '1';
    p[206] = '1';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_12(void) {
    char *p = (char *)0xb8000;
    p[208] = '1';
    p[210] = '2';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_13(void) {
    char *p = (char *)0xb8000;
    p[212] = '1';
    p[214] = '3';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_14(void) {
    char *p = (char *)0xb8000;
    p[216] = '1';
    p[218] = '4';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_15(void) {
    char *p = (char *)0xb8000;
    p[220] = '1';
    p[222] = '5';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_16(void) {
    char *p = (char *)0xb8000;
    p[224] = '1';
    p[226] = '6';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_17(void) {
    char *p = (char *)0xb8000;
    p[228] = '1';
    p[230] = '7';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_18(void) {
    char *p = (char *)0xb8000;
    p[232] = '1';
    p[234] = '8';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_19(void) {
    char *p = (char *)0xb8000;
    p[236] = '1';
    p[238] = '9';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_20(void) {
    char *p = (char *)0xb8000;
    p[240] = '2';
    p[242] = '0';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_21(void) {
    char *p = (char *)0xb8000;
    p[244] = '2';
    p[246] = '1';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_22(void) {
    char *p = (char *)0xb8000;
    p[248] = '2';
    p[250] = '2';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_23(void) {
    char *p = (char *)0xb8000;
    p[252] = '2';
    p[254] = '3';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_24(void) {
    char *p = (char *)0xb8000;
    p[256] = '2';
    p[258] = '4';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_25(void) {
    char *p = (char *)0xb8000;
    p[260] = '2';
    p[262] = '5';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_26(void) {
    char *p = (char *)0xb8000;
    p[264] = '2';
    p[266] = '6';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_27(void) {
    char *p = (char *)0xb8000;
    p[268] = '2';
    p[270] = '7';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_28(void) {
    char *p = (char *)0xb8000;
    p[272] = '2';
    p[274] = '8';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_29(void) {
    char *p = (char *)0xb8000;
    p[276] = '2';
    p[278] = '9';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_30(void) {
    char *p = (char *)0xb8000;
    p[280] = '3';
    p[282] = '0';
    __asm__ volatile ("add esp, 4"); // remove error code from stack
    __asm__ volatile ("iret");
}

__attribute__((naked))
void exception_handler_31(void) {
    char *p = (char *)0xb8000;
    p[284] = '3';
    p[286] = '1';
    __asm__ volatile ("iret");
}

__attribute__((naked))
void irq_handler_01(void) {
    char *p = (char *)0xb8000;
    p[288] = '#';
    __asm__ volatile ("iret");
}

void init_idt(void) {
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

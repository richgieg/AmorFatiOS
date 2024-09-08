#include <idt.h>
#include <bugcheck.h>
#include <process.h>

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

static void (*irq_handlers[16])();

__attribute__((naked))
static void exception_handler_00(void) {
    exception(0);
}

__attribute__((naked))
static void exception_handler_01(void) {
    exception(1);
}

__attribute__((naked))
static void exception_handler_02(void) {
    exception(2);
}

__attribute__((naked))
static void exception_handler_03(void) {
    exception(3);
}

__attribute__((naked))
static void exception_handler_04(void) {
    exception(4);
}

__attribute__((naked))
static void exception_handler_05(void) {
    exception(5);
}

__attribute__((naked))
static void exception_handler_06(void) {
    exception(6);
}

__attribute__((naked))
static void exception_handler_07(void) {
    exception(7);
}

__attribute__((naked))
static void exception_handler_08(void) {
    __asm__(
        "push 8\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_09(void) {
    exception(9);
}

__attribute__((naked))
static void exception_handler_10(void) {
    __asm__(
        "push 10\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_11(void) {
    __asm__(
        "push 11\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_12(void) {
    __asm__(
        "push 12\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_13(void) {
    __asm__(
        "push 13\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_14(void) {
    __asm__(
        "push 14\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_16(void) {
    exception(16);
}

__attribute__((naked))
static void exception_handler_17(void) {
    __asm__(
        "push 17\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_18(void) {
    exception(18);
}

__attribute__((naked))
static void exception_handler_19(void) {
    exception(19);
}

__attribute__((naked))
static void interrupt_handler_00(void) {
    __asm__(
        "pushad;"
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_00__skip_segment_init;"
        "mov ax, 0x10;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_00__skip_segment_init:"
    );
    if (irq_handlers[0]) irq_handlers[0]();
    __asm__(
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_00__skip_segment_restore;"
        "mov ax, 0x23;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_00__skip_segment_restore:"
        "popad;"
        "iret;"
    );
}

__attribute__((naked))
static void interrupt_handler_01(void) {
    __asm__(
        "pushad;"
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_01__skip_segment_init;"
        "mov ax, 0x10;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_01__skip_segment_init:"
    );
    if (irq_handlers[1]) irq_handlers[1]();
    __asm__(
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_01__skip_segment_restore;"
        "mov ax, 0x23;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_01__skip_segment_restore:"
        "popad;"
        "iret;"
    );
}

__attribute__((naked))
static void interrupt_handler_02(void) {
    BUGCHECK("Spurious IRQ 2.");
}

__attribute__((naked))
static void interrupt_handler_03(void) {
    BUGCHECK("Spurious IRQ 3.");
}

__attribute__((naked))
static void interrupt_handler_04(void) {
    BUGCHECK("Spurious IRQ 0x04.");
}

__attribute__((naked))
static void interrupt_handler_05(void) {
    BUGCHECK("Spurious IRQ 5.");
}

__attribute__((naked))
static void interrupt_handler_06(void) {
    BUGCHECK("Spurious IRQ 6.");
}

__attribute__((naked))
static void interrupt_handler_07(void) {
    BUGCHECK("Spurious IRQ 7.");
}

__attribute__((naked))
static void interrupt_handler_08(void) {
    BUGCHECK("Spurious IRQ 8.");
}

__attribute__((naked))
static void interrupt_handler_09(void) {
    BUGCHECK("Spurious IRQ 9.");
}

__attribute__((naked))
static void interrupt_handler_10(void) {
    BUGCHECK("Spurious IRQ 10.");
}

__attribute__((naked))
static void interrupt_handler_11(void) {
    __asm__(
        "pushad;"
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_11__skip_segment_init;"
        "mov ax, 0x10;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_11__skip_segment_init:"
    );
    if (irq_handlers[11]) irq_handlers[11]();
    __asm__(
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_11__skip_segment_restore;"
        "mov ax, 0x23;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_11__skip_segment_restore:"
        "popad;"
        "iret;"
    );
}

__attribute__((naked))
static void interrupt_handler_12(void) {
    __asm__(
        "pushad;"
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_12__skip_segment_init;"
        "mov ax, 0x10;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_12__skip_segment_init:"
    );
    if (irq_handlers[12]) irq_handlers[12]();
    __asm__(
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_12__skip_segment_restore;"
        "mov ax, 0x23;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_12__skip_segment_restore:"
        "popad;"
        "iret;"
    );
}

__attribute__((naked))
static void interrupt_handler_13(void) {
    BUGCHECK("Spurious IRQ 13.");
}

__attribute__((naked))
static void interrupt_handler_14(void) {
    BUGCHECK("Spurious IRQ 14.");
}

__attribute__((naked))
static void interrupt_handler_15(void) {
    BUGCHECK("Spurious IRQ 15.");
}

__attribute__((naked))
static void interrupt_handler_128(void) {
    __asm__(
        "pushad;"
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_128__skip_segment_init;"
        "mov ax, 0x10;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_128__skip_segment_init:"
    );
    process_switch(PROCESS_STATE_WAITING_FOR_KEY_EVENT);
    __asm__(
        "cmpd [esp + 36], 0x1b;"
        "jne interrupt_handler_128__skip_segment_restore;"
        "mov ax, 0x23;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "interrupt_handler_128__skip_segment_restore:"
        "popad;"
        "iret;"
    );
}

void idt_set_irq_handler(u8 irq, void (*handler)()) {
    irq_handlers[irq] = handler;
}

static void idt_set_descriptor(u8 vector, void *isr, u8 flags) {
    struct idt_entry *descriptor = &idt[vector];
    descriptor->isr_low = (u32)isr & 0xffff;
    descriptor->kernel_cs = 0x08;
    descriptor->attributes = flags;
    descriptor->isr_high = (u32)isr >> 16;
    descriptor->reserved = 0;
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

    idt_set_descriptor(32, interrupt_handler_00, 0x8e);
    idt_set_descriptor(33, interrupt_handler_01, 0x8e);
    idt_set_descriptor(34, interrupt_handler_02, 0x8e);
    idt_set_descriptor(35, interrupt_handler_03, 0x8e);
    idt_set_descriptor(36, interrupt_handler_04, 0x8e);
    idt_set_descriptor(37, interrupt_handler_05, 0x8e);
    idt_set_descriptor(38, interrupt_handler_06, 0x8e);
    idt_set_descriptor(39, interrupt_handler_07, 0x8e);
    idt_set_descriptor(40, interrupt_handler_08, 0x8e);
    idt_set_descriptor(41, interrupt_handler_09, 0x8e);
    idt_set_descriptor(42, interrupt_handler_10, 0x8e);
    idt_set_descriptor(43, interrupt_handler_11, 0x8e);
    idt_set_descriptor(44, interrupt_handler_12, 0x8e);
    idt_set_descriptor(45, interrupt_handler_13, 0x8e);
    idt_set_descriptor(46, interrupt_handler_14, 0x8e);
    idt_set_descriptor(47, interrupt_handler_15, 0x8e);

    idt_set_descriptor(128, interrupt_handler_128, 0x8e);

    __asm__("lidt %0" : : "m"(idtr)); // load the new IDT
}

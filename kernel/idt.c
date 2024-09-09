#include <idt.h>
#include <bugcheck.h>
#include <process.h>
#include <sys.h>

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
static struct idt_entry idt[IDT_MAX_DESCRIPTORS]; // array of IDT entries aligned for performance

static void (*irq_handlers[16])();

__attribute__((naked))
static void exception_handler_0x00(void) {
    exception(0x00);
}

__attribute__((naked))
static void exception_handler_0x01(void) {
    exception(0x01);
}

__attribute__((naked))
static void exception_handler_0x02(void) {
    exception(0x02);
}

__attribute__((naked))
static void exception_handler_0x03(void) {
    exception(0x03);
}

__attribute__((naked))
static void exception_handler_0x04(void) {
    exception(0x04);
}

__attribute__((naked))
static void exception_handler_0x05(void) {
    exception(0x05);
}

__attribute__((naked))
static void exception_handler_0x06(void) {
    exception(0x06);
}

__attribute__((naked))
static void exception_handler_0x07(void) {
    exception(0x07);
}

__attribute__((naked))
static void exception_handler_0x08(void) {
    __asm__(
        "push 0x08\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x09(void) {
    exception(0x09);
}

__attribute__((naked))
static void exception_handler_0x0a(void) {
    __asm__(
        "push 0x0a\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x0b(void) {
    __asm__(
        "push 0x0b\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x0c(void) {
    __asm__(
        "push 0x0c\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x0d(void) {
    __asm__(
        "push 0x0d\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x0e(void) {
    __asm__(
        "push 0x0e\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x10(void) {
    exception(0x10);
}

__attribute__((naked))
static void exception_handler_0x11(void) {
    __asm__(
        "push 0x11\n"
        "call exception_with_code"
    );
}

__attribute__((naked))
static void exception_handler_0x12(void) {
    exception(0x12);
}

__attribute__((naked))
static void exception_handler_0x13(void) {
    exception(0x13);
}

__attribute__((naked))
static void set_segment_regs_if_needed(void) {
    __asm__(
        "cmpd [esp + 8], 0x1b;"
        "jne set_segment_regs_if_needed__return;"
        "push eax;"
        "mov ax, 0x10;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "pop eax;"
        "set_segment_regs_if_needed__return:"
        "ret;"
    );
}

__attribute__((naked))
static void restore_segment_regs_if_needed(void) {
    __asm__(
        "cmpd [esp + 8], 0x1b;"
        "jne restore_segment_regs_if_needed__return;"
        "push eax;"
        "mov ax, 0x23;"
        "mov ds, ax;"
        "mov es, ax;"
        "mov fs, ax;"
        "mov gs, ax;"
        "pop eax;"
        "restore_segment_regs_if_needed__return:"
        "ret;"
    );
}

// IRQ 0
__attribute__((naked))
static void interrupt_handler_0x20(void) {
    set_segment_regs_if_needed();
    __asm__("pushad");
    if (irq_handlers[0]) irq_handlers[0]();
    __asm__("popad");
    restore_segment_regs_if_needed();
    __asm__("iret");
}

// IRQ 1
__attribute__((naked))
static void interrupt_handler_0x21(void) {
    set_segment_regs_if_needed();
    __asm__("pushad");
    if (irq_handlers[1]) irq_handlers[1]();
    __asm__("popad");
    restore_segment_regs_if_needed();
    __asm__("iret");
}

// IRQ 2
__attribute__((naked))
static void interrupt_handler_0x22(void) {
    BUGCHECK("Spurious IRQ 2.");
}

// IRQ 3
__attribute__((naked))
static void interrupt_handler_0x23(void) {
    BUGCHECK("Spurious IRQ 3.");
}

// IRQ 4
__attribute__((naked))
static void interrupt_handler_0x24(void) {
    BUGCHECK("Spurious IRQ 0x04.");
}

// IRQ 5
__attribute__((naked))
static void interrupt_handler_0x25(void) {
    BUGCHECK("Spurious IRQ 5.");
}

// IRQ 6
__attribute__((naked))
static void interrupt_handler_0x26(void) {
    BUGCHECK("Spurious IRQ 6.");
}

// IRQ 7
__attribute__((naked))
static void interrupt_handler_0x27(void) {
    BUGCHECK("Spurious IRQ 7.");
}

// IRQ 8
__attribute__((naked))
static void interrupt_handler_0x28(void) {
    BUGCHECK("Spurious IRQ 8.");
}

// IRQ 9
__attribute__((naked))
static void interrupt_handler_0x29(void) {
    BUGCHECK("Spurious IRQ 9.");
}

// IRQ 10
__attribute__((naked))
static void interrupt_handler_0x2a(void) {
    BUGCHECK("Spurious IRQ 10.");
}

// IRQ 11
__attribute__((naked))
static void interrupt_handler_0x2b(void) {
    set_segment_regs_if_needed();
    __asm__("pushad");
    if (irq_handlers[11]) irq_handlers[11]();
    __asm__("popad");
    restore_segment_regs_if_needed();
    __asm__("iret");
}

// IRQ 12
__attribute__((naked))
static void interrupt_handler_0x2c(void) {
    set_segment_regs_if_needed();
    __asm__("pushad");
    if (irq_handlers[12]) irq_handlers[12]();
    __asm__("popad");
    restore_segment_regs_if_needed();
    __asm__("iret");
}

// IRQ 13
__attribute__((naked))
static void interrupt_handler_0x2d(void) {
    BUGCHECK("Spurious IRQ 13.");
}

// IRQ 14
__attribute__((naked))
static void interrupt_handler_0x2e(void) {
    BUGCHECK("Spurious IRQ 14.");
}

// IRQ 15
__attribute__((naked))
static void interrupt_handler_0x2f(void) {
    BUGCHECK("Spurious IRQ 15.");
}

// INT 0x80
__attribute__((naked))
static void interrupt_handler_0x80(void) {
    set_segment_regs_if_needed();
    __asm__("sub esp, 4");
    __asm__("pushad");
    sys_dispatch();
    __asm__("mov [esp + 32], eax");
    __asm__("popad");
    __asm__("pop eax");
    restore_segment_regs_if_needed();
    __asm__("iret");
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
    idtr.base = (u32)idt;
    idtr.limit = sizeof(idt) - 1;

    idt_set_descriptor(0x00, exception_handler_0x00, 0x8e);
    idt_set_descriptor(0x01, exception_handler_0x01, 0x8e);
    idt_set_descriptor(0x02, exception_handler_0x02, 0x8e);
    idt_set_descriptor(0x03, exception_handler_0x03, 0x8e);
    idt_set_descriptor(0x04, exception_handler_0x04, 0x8e);
    idt_set_descriptor(0x05, exception_handler_0x05, 0x8e);
    idt_set_descriptor(0x06, exception_handler_0x06, 0x8e);
    idt_set_descriptor(0x07, exception_handler_0x07, 0x8e);
    idt_set_descriptor(0x08, exception_handler_0x08, 0x8e);
    idt_set_descriptor(0x09, exception_handler_0x09, 0x8e);
    idt_set_descriptor(0x0a, exception_handler_0x0a, 0x8e);
    idt_set_descriptor(0x0b, exception_handler_0x0b, 0x8e);
    idt_set_descriptor(0x0c, exception_handler_0x0c, 0x8e);
    idt_set_descriptor(0x0d, exception_handler_0x0d, 0x8e);
    idt_set_descriptor(0x0e, exception_handler_0x0e, 0x8e);
    idt_set_descriptor(0x10, exception_handler_0x10, 0x8e);
    idt_set_descriptor(0x11, exception_handler_0x11, 0x8e);
    idt_set_descriptor(0x12, exception_handler_0x12, 0x8e);
    idt_set_descriptor(0x13, exception_handler_0x13, 0x8e);

    idt_set_descriptor(0x20, interrupt_handler_0x20, 0x8e);
    idt_set_descriptor(0x21, interrupt_handler_0x21, 0x8e);
    idt_set_descriptor(0x22, interrupt_handler_0x22, 0x8e);
    idt_set_descriptor(0x23, interrupt_handler_0x23, 0x8e);
    idt_set_descriptor(0x24, interrupt_handler_0x24, 0x8e);
    idt_set_descriptor(0x25, interrupt_handler_0x25, 0x8e);
    idt_set_descriptor(0x26, interrupt_handler_0x26, 0x8e);
    idt_set_descriptor(0x27, interrupt_handler_0x27, 0x8e);
    idt_set_descriptor(0x28, interrupt_handler_0x28, 0x8e);
    idt_set_descriptor(0x29, interrupt_handler_0x29, 0x8e);
    idt_set_descriptor(0x2a, interrupt_handler_0x2a, 0x8e);
    idt_set_descriptor(0x2b, interrupt_handler_0x2b, 0x8e);
    idt_set_descriptor(0x2c, interrupt_handler_0x2c, 0x8e);
    idt_set_descriptor(0x2d, interrupt_handler_0x2d, 0x8e);
    idt_set_descriptor(0x2e, interrupt_handler_0x2e, 0x8e);
    idt_set_descriptor(0x2f, interrupt_handler_0x2f, 0x8e);

    idt_set_descriptor(0x80, interrupt_handler_0x80, 0xee);

    __asm__("lidt %0" : : "m"(idtr)); // load the new IDT
}

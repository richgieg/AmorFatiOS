#include "keyboard.h"
#include "port.h"
#include "idt.h"
#include "ps2.h"
#include "pic.h"
#include "vga.h"

#define EXTENDED_KEY_PREFIX 0xe0
#define BREAK_CODE_PREFIX 0xf0

static uint8_t is_break;
static uint8_t is_extended;

static void handle_key_press(uint8_t scancode, uint8_t is_extended) {
    vga_putb(scancode);
    if (is_extended) {
        vga_putc('E');
    }
    vga_puts("P ");
}

static void handle_key_release(uint8_t scancode, uint8_t is_extended) {
    vga_putb(scancode);
    if (is_extended) {
        vga_putc('E');
    }
    vga_puts("R ");
}

__attribute__((naked))
static void interrupt_service_routine(void) {
    uint8_t data = inb(PS2_DATA_PORT);

    if (data == EXTENDED_KEY_PREFIX) {
        is_extended = 1;
    } else if (data == BREAK_CODE_PREFIX) {
        is_break = 1;
    } else {
        if (is_break) {
            handle_key_release(data, is_extended);
            is_break = 0;
        } else {
            handle_key_press(data, is_extended);
        }
        is_extended = 0;
    }

    outb(PIC1_COMMAND, PIC_EOI);
    __asm__("iret");
}

void keyboard_init(void) {
    idt_set_descriptor(IRQ1_INTERRUPT, interrupt_service_routine, 0x8e);
    pic_unmask_irq(1);
}

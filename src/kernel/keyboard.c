#include <keyboard.h>
#include <port.h>
#include <idt.h>
#include <ps2.h>
#include <pic.h>
#include <console.h>
#include <mm.h>

#define EXTENDED_KEY_PREFIX 0xe0
#define BREAK_CODE_PREFIX 0xf0

static u8 is_break;
static u8 is_extended;

static void handle_key_press(u8 scancode, u8 is_extended) {
    u16 scancodew = 0;
    if (is_extended) {
        scancodew = EXTENDED_KEY_PREFIX << 8;
    }
    scancodew |= scancode;
    console_dbg_puts("Key Pressed: ");
    console_dbg_putw(scancodew);
    console_dbg_putc('\n');

    switch (scancodew) {
        case SC_LEFT:
            console_prev();
            break;
        case SC_RIGHT:
            console_next();
            break;
        default:
            console_key_press(scancodew);
            break;
    }
}

static void handle_key_release(u8 scancode, u8 is_extended) {
    u16 scancodew = 0;
    if (is_extended) {
        scancodew = EXTENDED_KEY_PREFIX << 8;
    }
    scancodew |= scancode;
    console_dbg_puts("Key Released: ");
    console_dbg_putw(scancodew);
    console_dbg_putc('\n');

    switch (scancodew) {
        case SC_LEFT:
            break;
        case SC_RIGHT:
            break;
        default:
            console_key_release(scancodew);
            break;
    }
}

static void interrupt_service_routine(void) {
    u8 data = inb(PS2_DATA_PORT);

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
}

void keyboard_init(void) {
    idt_set_irq_handler(1, interrupt_service_routine);
    pic_unmask_irq(1);
}

#include <device/keyboard.h>
#include <port.h>
#include <idt.h>
#include <device/ps2.h>
#include <device/pic.h>
#include <console.h>
#include <types.h>

#define EXTENDED_KEY_PREFIX 0xe0
#define BREAK_CODE_PREFIX 0xf0

static bool is_break;
static bool is_extended;

static bool is_left_alt_down;
static bool is_right_alt_down;
static bool is_left_shift_down;
static bool is_right_shift_down;

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
        case SC_LEFT_ALT:
            is_left_alt_down = true;
            console_key_press(scancodew);
            break;
        case SC_RIGHT_ALT:
            is_right_alt_down = true;
            console_key_press(scancodew);
            break;
        case SC_LEFT_SHIFT:
            is_left_shift_down = true;
            console_key_press(scancodew);
            break;
        case SC_RIGHT_SHIFT:
            is_right_shift_down = true;
            console_key_press(scancodew);
            break;
        case SC_TAB:
            if (is_left_alt_down || is_right_alt_down) {
                if (is_left_shift_down || is_right_shift_down) {
                    console_prev();
                } else {
                    console_next();
                }
            } else {
                console_key_press(scancodew);
            }
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
        case SC_LEFT_ALT:
            is_left_alt_down = false;
            console_key_press(scancodew);
            break;
        case SC_RIGHT_ALT:
            is_right_alt_down = false;
            console_key_press(scancodew);
            break;
        case SC_LEFT_SHIFT:
            is_left_shift_down = false;
            console_key_press(scancodew);
            break;
        case SC_RIGHT_SHIFT:
            is_right_shift_down = false;
            console_key_press(scancodew);
            break;
        default:
            console_key_release(scancodew);
            break;
    }
}

static void interrupt_service_routine(void) {
    u8 data = inb(PS2_DATA_PORT);

    if (data == EXTENDED_KEY_PREFIX) {
        is_extended = true;
    } else if (data == BREAK_CODE_PREFIX) {
        is_break = true;
    } else {
        if (is_break) {
            handle_key_release(data, is_extended);
            is_break = false;
        } else {
            handle_key_press(data, is_extended);
        }
        is_extended = false;
    }

    outb(PIC1_COMMAND, PIC_EOI);
}

void keyboard_init(void) {
    idt_set_irq_handler(1, interrupt_service_routine);
    pic_unmask_irq(1);
}

#include "keyboard.h"
#include "port.h"
#include "idt.h"
#include "ps2.h"
#include "pic.h"
#include "console.h"
#include "mm.h"

#define EXTENDED_KEY_PREFIX 0xe0
#define BREAK_CODE_PREFIX 0xf0

#define SC_F11 0x0078
#define SC_F12 0x0007

#define SC_UP 0xe075
#define SC_DOWN 0xe072
#define SC_LEFT 0xe06b
#define SC_RIGHT 0xe074

#define SC_HOME 0xe06c
#define SC_END 0xe069
#define SC_PAGEUP 0xe07d
#define SC_PAGEDOWN 0xe07a
#define SC_INSERT 0xe070
#define SC_DELETE 0xe071

static u8 is_break;
static u8 is_extended;

static void handle_key_press(u8 scancode, u8 is_extended) {
    u16 scancodew = 0;
    if (is_extended) {
        scancodew = EXTENDED_KEY_PREFIX << 8;
    }
    scancodew |= scancode;
    // console_key_press(scancodew);
    // console_dbg_putw(scancodew);
    // console_dbg_putc('\n');

    switch (scancodew) {
        case SC_F11:
            console_prev();
            break;
        case SC_F12:
            console_next();
            break;
        // case SC_LEFT:
        //     mm_mdump_prev_byte();
        //     break;
        // case SC_RIGHT:
        //     mm_mdump_next_byte();
        //     break;
        // case SC_UP:
        //     mm_mdump_prev_line();
        //     break;
        // case SC_DOWN:
        //     mm_mdump_next_line();
        //     break;
        // case SC_PAGEUP:
        //     mm_mdump_prev_page();
        //     break;
        // case SC_PAGEDOWN:
        //     mm_mdump_next_page();
        //     break;
        // case SC_HOME:
        //     mm_mdump_prev_mb();
        //     break;
        // case SC_END:
        //     mm_mdump_next_mb();
        //     break;
    }
}

static void handle_key_release(u8 scancode, u8 is_extended) {
    u16 scancodew = 0;
    if (is_extended) {
        scancodew = EXTENDED_KEY_PREFIX << 8;
    }
    scancodew |= scancode;
    // console_key_release(scancodew);
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

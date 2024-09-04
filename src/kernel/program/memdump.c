#include <program/memdump.h>
#include <mm.h>
#include <console.h>
#include <device/keyboard.h>

void memdump(void) {
    mm_show_mdump();
    while (1) {
        struct key_event ke = console_read_key_event();
        if (!ke.is_release) {
            switch (ke.scancode) {
                case SC_LEFT:
                    mm_mdump_prev_byte();
                    break;
                case SC_RIGHT:
                    mm_mdump_next_byte();
                    break;
                case SC_UP:
                    mm_mdump_prev_line();
                    break;
                case SC_DOWN:
                    mm_mdump_next_line();
                    break;
                case SC_PAGEUP:
                    mm_mdump_prev_page();
                    break;
                case SC_PAGEDOWN:
                    mm_mdump_next_page();
                    break;
                case SC_HOME:
                    mm_mdump_prev_mb();
                    break;
                case SC_END:
                    mm_mdump_next_mb();
                    break;
            }
        }
    }
}

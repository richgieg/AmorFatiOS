#include <program/memdump.h>
#include <console.h>
#include <sys.h>

#define BYTES_PER_LINE 16
#define BYTES_PER_PAGE 4096
#define BYTES_PER_MB 1024 * 1024

static u32 current_address;

static void show_memdump(void);
static void next_byte(void);
static void prev_byte(void);
static void next_line(void);
static void prev_line(void);
static void next_page(void);
static void prev_page(void);
static void next_mb(void);
static void prev_mb(void);

void memdump(void) {
    current_address = 0;

    show_memdump();
    while (true) {
        struct key_event ke;
        sys_console_read_key_event(&ke);

        if (!ke.is_release) {
            switch (ke.scancode) {
                case SC_LEFT:
                    prev_byte();
                    break;
                case SC_RIGHT:
                    next_byte();
                    break;
                case SC_UP:
                    prev_line();
                    break;
                case SC_DOWN:
                    next_line();
                    break;
                case SC_PAGEUP:
                    prev_page();
                    break;
                case SC_PAGEDOWN:
                    next_page();
                    break;
                case SC_HOME:
                    prev_mb();
                    break;
                case SC_END:
                    next_mb();
                    break;
                case SC_Q:
                    sys_console_putc('\n');
                    sys_process_exit();
                    break;
            }
        }
    }
}

static void show_memdump(void) {
    volatile u8 *memory_ptr = (volatile u8 *)current_address;
    sys_console_set_pos(0, 0);
    int num_rows = sys_console_get_num_rows();

    for (int k = 0; k < num_rows; k++) {
        sys_console_putdw((u32)memory_ptr);
        sys_console_puts("    ");

        for (int i = 0; i < BYTES_PER_LINE; i++) {
            sys_console_putb(*memory_ptr);
            sys_console_putc(' ');
            memory_ptr++;
        }

        sys_console_puts("   ");
        memory_ptr -= BYTES_PER_LINE;

        for (int i = 0; i < BYTES_PER_LINE; i++) {
            sys_console_writec(*memory_ptr);
            memory_ptr++;
        }

        if (k < num_rows - 1) {
            sys_console_putc('\n');
        }
    }
}

static void next_byte(void) {
    current_address += 1;
    show_memdump();
}

static void prev_byte(void) {
    current_address -= 1;
    show_memdump();
}

static void next_line(void) {
    current_address += BYTES_PER_LINE;
    show_memdump();
}

static void prev_line(void) {
    current_address -= BYTES_PER_LINE;
    show_memdump();
}

static void next_page(void) {
    current_address += BYTES_PER_PAGE;
    show_memdump();
}

static void prev_page(void) {
    current_address -= BYTES_PER_PAGE;
    show_memdump();
}

static void next_mb(void) {
    current_address += BYTES_PER_MB;
    show_memdump();
}

static void prev_mb(void) {
    current_address -= BYTES_PER_MB;
    show_memdump();
}

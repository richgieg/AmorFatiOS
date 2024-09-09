#include <program/memdump.h>
#include <console.h>
#include <device/keyboard.h>

#define BYTES_PER_LINE 16
#define BYTES_PER_PAGE 4096
#define BYTES_PER_MB 1024 * 1024

static u32 current_address = 0x00000000;

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
    show_memdump();
    while (true) {
        struct key_event ke;

        __asm__(
            "mov eax, %0;"
            "mov ebx, %1;"
            "int 0x80;"
            :
            : "eax" (0x0016), "ebx" (&ke)
        );

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
            }
        }
    }
}

static void show_memdump(void) {
    volatile u8 *memory_ptr = (volatile u8 *)current_address;
    console_set_pos(0, 0);
    int num_rows;
    console_get_num_rows(&num_rows);

    for (int k = 0; k < num_rows; k++) {
        console_putdw((u32)memory_ptr);
        console_puts("    ");

        for (int i = 0; i < BYTES_PER_LINE; i++) {
            console_putb(*memory_ptr);
            console_putc(' ');
            memory_ptr++;
        }

        console_puts("   ");
        memory_ptr -= BYTES_PER_LINE;

        for (int i = 0; i < BYTES_PER_LINE; i++) {
            console_writec(*memory_ptr);
            memory_ptr++;
        }

        if (k < num_rows - 1) {
            console_putc('\n');
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

#include "console.h"
#include "process.h"
#include "types.h"

#define VGA_ADDRESS 0xb8000

struct console {
    u16 buffer[CONSOLE_COLUMNS * CONSOLE_ROWS];
    enum console_color bg_color;
    enum console_color text_color;
    u8 col;
    u8 row;
} __attribute__((aligned(4096)));

static struct console consoles[MAX_PROCESSES];
static int current_console_index;

static volatile u16 *vga_buffer = (volatile u16 *)VGA_ADDRESS;

static inline u16 vga_entry(
    char character, enum console_color bg_color, enum console_color text_color
) {
    u8 attributes = bg_color << 4 | text_color;
    return (u8)character | (u16)attributes << 8;
}

void console_init(void) {
    enum console_color bg_color = CONSOLE_COLOR_CYAN;
    enum console_color text_color = CONSOLE_COLOR_BRIGHT_WHITE;
    u16 entry = vga_entry(0, bg_color, text_color);

    for (int i = 0; i < MAX_PROCESSES; i++) {
        consoles[i].bg_color = bg_color;
        consoles[i].text_color = text_color;
        for (int j = 0; j < CONSOLE_ROWS * CONSOLE_COLUMNS; j++) {
            consoles[i].buffer[j] = entry;
        }
    }

    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        vga_buffer[i] = entry;
    }
}

void console_clear(void) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u16 entry = vga_entry(0, con->bg_color, con->text_color);
    
    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        con->buffer[i] = entry;
    }

    con->col = 0;
    con->row = 0;

    if (current_process_index == current_console_index) {
        for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
            vga_buffer[i] = con->buffer[i];
        }
    }
}

static void scroll_one_line(void) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];

    for (int i = 0; i < (CONSOLE_ROWS - 1) * CONSOLE_COLUMNS; i++) {
        con->buffer[i] = con->buffer[i + CONSOLE_COLUMNS];
    }

    for (int i = 0; i < CONSOLE_COLUMNS; i++) {
        con->buffer[(CONSOLE_ROWS - 1) * CONSOLE_COLUMNS + i] = vga_entry(0, con->bg_color, con->text_color);
    }

    if (current_process_index == current_console_index) {
        for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
            vga_buffer[i] = con->buffer[i];
        }
    }
}

static void go_to_next_line(void) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    con->col = 0;
    con->row++;
    if (con->row >= CONSOLE_ROWS) {
        con->row = CONSOLE_ROWS - 1;
        scroll_one_line();
    }
}

void console_putc(char c) {
    if (c == '\n') {
        go_to_next_line();
    } else {
        int current_process_index = process_get_current_index();
        struct console *con = &consoles[current_process_index];
        u16 entry = vga_entry(c, con->bg_color, con->text_color);
        con->buffer[con->row * CONSOLE_COLUMNS + con->col] = entry;

        if (current_process_index == current_console_index) {
            vga_buffer[con->row * CONSOLE_COLUMNS + con->col] = entry;
        }
    }
}

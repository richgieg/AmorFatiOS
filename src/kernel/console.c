#include "console.h"
#include "process.h"
#include "types.h"

#define VGA_ADDRESS 0xb8000

static const char hex_digits[] = "0123456789ABCDEF";

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

enum console_color console_get_bg_color() {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    return con->bg_color;
}

void console_set_bg_color(enum console_color bg_color) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    con->bg_color = bg_color;
}

enum console_color console_get_text_color() {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    return con->text_color;
}

void console_set_text_color(enum console_color text_color) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    con->text_color = text_color;
}

void console_set_pos(u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    con->col = col;
    con->row = row;
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

void console_writec(char c) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u16 entry = vga_entry(c, con->bg_color, con->text_color);
    con->buffer[con->row * CONSOLE_COLUMNS + con->col] = entry;

    if (current_process_index == current_console_index) {
        vga_buffer[con->row * CONSOLE_COLUMNS + con->col] = entry;
    }

    con->col++;
    if (con->col >= CONSOLE_COLUMNS) {
        go_to_next_line();
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

        con->col++;
        if (con->col >= CONSOLE_COLUMNS) {
            go_to_next_line();
        }
    }
}

void console_puts(const char *str) {
    while (*str) {
        console_putc(*str);
        str++;
    }
}

void console_putb(u8 b) {
    char str[3];
    str[2] = '\0';
    str[1] = hex_digits[b & 0xf];
    b >>= 4;
    str[0] = hex_digits[b & 0xf];
    console_puts(str);
}

void console_putw(u16 w) {
    char str[5];
    str[4] = '\0';
    str[3] = hex_digits[w & 0xf];
    w >>= 4;
    str[2] = hex_digits[w & 0xf];
    w >>= 4;
    str[1] = hex_digits[w & 0xf];
    w >>= 4;
    str[0] = hex_digits[w & 0xf];
    console_puts(str);
}

void console_putdw(u32 dw) {
    char str[9];
    str[8] = '\0';
    str[7] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[6] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[5] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[4] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[3] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[2] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[1] = hex_digits[dw & 0xf];
    dw >>= 4;
    str[0] = hex_digits[dw & 0xf];
    console_puts(str);
}

void console_putqw(u64 qw) {
    char str[17];
    str[16] = '\0';
    str[15] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[14] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[13] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[12] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[11] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[10] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[9] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[8] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[7] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[6] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[5] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[4] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[3] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[2] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[1] = hex_digits[qw & 0xf];
    qw >>= 4;
    str[0] = hex_digits[qw & 0xf];
    console_puts(str);
}

void console_putp(void *p) {
    console_putdw((u32)p);
}

void console_putc_at(char c, u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putc(c);
    con->col = old_col;
    con->row = old_row;
}

void console_puts_at(const char *str, u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_puts(str);
    con->col = old_col;
    con->row = old_row;
}

void console_putb_at(u8 b, u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putb(b);
    con->col = old_col;
    con->row = old_row;
}

void console_putw_at(u16 w, u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putw(w);
    con->col = old_col;
    con->row = old_row;
}

void console_putdw_at(u32 dw, u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putdw(dw);
    con->col = old_col;
    con->row = old_row;
}

void console_putqw_at(u64 qw, u8 col, u8 row) {
    int current_process_index = process_get_current_index();
    struct console *con = &consoles[current_process_index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putqw(qw);
    con->col = old_col;
    con->row = old_row;
}

void console_putp_at(void *p, u8 col, u8 row) {
    console_putdw_at((u32)p, col, row);
}

void console_dump_chars(void) {
    u8 x = 0;
    u8 y = 0;
    char c = 0;
    for (int i = 0; i < 256; i++) {
        console_putc_at(c, x, y);
        c++;
        x++;
        if (x >= CONSOLE_COLUMNS) {
            x = 0;
            y++;
        }
    }
}

void console_next(void) {
    current_console_index = (current_console_index + 1) % MAX_PROCESSES;
    struct console *con = &consoles[current_console_index];
    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        vga_buffer[i] = con->buffer[i];
    }
}

void console_prev(void) {
    current_console_index = (current_console_index + MAX_PROCESSES - 1) % MAX_PROCESSES;
    struct console *con = &consoles[current_console_index];
    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        vga_buffer[i] = con->buffer[i];
    }
}

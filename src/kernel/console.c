#include "console.h"
#include "process.h"
#include "types.h"
#include "bugcheck.h"

#define KEY_EVENT_BUFFER_MAX_EVENTS 256
#define VGA_ADDRESS 0xb8000
#define MAX_CONSOLES (MAX_PROCESSES + 1)
#define DBG_CONSOLE_INDEX (MAX_CONSOLES - 1)

static const char hex_digits[] = "0123456789ABCDEF";

struct key_event_buffer {
    struct key_event events[KEY_EVENT_BUFFER_MAX_EVENTS];
    u8 head;
    u8 tail;
};

struct console {
    u16 buffer[CONSOLE_COLUMNS * CONSOLE_ROWS];
    enum console_color bg_color;
    enum console_color text_color;
    u8 col;
    u8 row;
    struct key_event_buffer keb;
} __attribute__((aligned(4096)));

static struct console consoles[MAX_CONSOLES];
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

    for (int i = 0; i < DBG_CONSOLE_INDEX; i++) {
        consoles[i].bg_color = bg_color;
        consoles[i].text_color = text_color;
        for (int j = 0; j < CONSOLE_ROWS * CONSOLE_COLUMNS; j++) {
            consoles[i].buffer[j] = entry;
        }
    }

    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        vga_buffer[i] = entry;
    }

    bg_color = CONSOLE_COLOR_BLACK;
    text_color = CONSOLE_COLOR_BRIGHT_WHITE;
    entry = vga_entry(0, bg_color, text_color);
    consoles[DBG_CONSOLE_INDEX].bg_color = bg_color;
    consoles[DBG_CONSOLE_INDEX].text_color = text_color;
    for (int j = 0; j < CONSOLE_ROWS * CONSOLE_COLUMNS; j++) {
        consoles[DBG_CONSOLE_INDEX].buffer[j] = entry;
    }
}

static void _console_clear(int index) {
    struct console *con = &consoles[index];
    u16 entry = vga_entry(0, con->bg_color, con->text_color);
    
    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        con->buffer[i] = entry;
    }

    con->col = 0;
    con->row = 0;
}

void console_clear(void) {
    _console_clear(process_get_current_index());
}

void console_dbg_clear(void) {
    _console_clear(DBG_CONSOLE_INDEX);
}

static enum console_color _console_get_bg_color(int index) {
    struct console *con = &consoles[index];
    return con->bg_color;
}

enum console_color console_get_bg_color() {
    return _console_get_bg_color(process_get_current_index());
}

enum console_color console_dbg_get_bg_color() {
    return _console_get_bg_color(DBG_CONSOLE_INDEX);
}

static void _console_set_bg_color(int index, enum console_color bg_color) {
    struct console *con = &consoles[index];
    con->bg_color = bg_color;
}

void console_set_bg_color(enum console_color bg_color) {
    _console_set_bg_color(process_get_current_index(), bg_color);
}

void console_dbg_set_bg_color(enum console_color bg_color) {
    _console_set_bg_color(DBG_CONSOLE_INDEX, bg_color);
}

static enum console_color _console_get_text_color(int index) {
    struct console *con = &consoles[index];
    return con->text_color;
}

enum console_color console_get_text_color() {
    return _console_get_text_color(process_get_current_index());
}

enum console_color console_dbg_get_text_color() {
    return _console_get_text_color(DBG_CONSOLE_INDEX);
}

static void _console_set_text_color(int index, enum console_color text_color) {
    struct console *con = &consoles[index];
    con->text_color = text_color;
}

void console_set_text_color(enum console_color text_color) {
    _console_set_text_color(process_get_current_index(), text_color);
}

void console_dbg_set_text_color(enum console_color text_color) {
    _console_set_text_color(DBG_CONSOLE_INDEX, text_color);
}

static void _console_set_pos(int index, u8 col, u8 row) {
    struct console *con = &consoles[index];
    con->col = col;
    con->row = row;
}

void console_set_pos(u8 col, u8 row) {
    _console_set_pos(process_get_current_index(), col, row);
}

void console_dbg_set_pos(u8 col, u8 row) {
    _console_set_pos(DBG_CONSOLE_INDEX, col, row);
}

static void scroll_one_line(int index) {
    struct console *con = &consoles[index];

    for (int i = 0; i < (CONSOLE_ROWS - 1) * CONSOLE_COLUMNS; i++) {
        con->buffer[i] = con->buffer[i + CONSOLE_COLUMNS];
    }

    for (int i = 0; i < CONSOLE_COLUMNS; i++) {
        con->buffer[(CONSOLE_ROWS - 1) * CONSOLE_COLUMNS + i] = vga_entry(0, con->bg_color, con->text_color);
    }
}

static void go_to_next_line(int index) {
    struct console *con = &consoles[index];
    con->col = 0;
    con->row++;
    if (con->row >= CONSOLE_ROWS) {
        con->row = CONSOLE_ROWS - 1;
        scroll_one_line(index);
    }
}

static void _console_writec(int index, char c) {
    struct console *con = &consoles[index];
    u16 entry = vga_entry(c, con->bg_color, con->text_color);
    con->buffer[con->row * CONSOLE_COLUMNS + con->col] = entry;

    con->col++;
    if (con->col >= CONSOLE_COLUMNS) {
        go_to_next_line(index);
    }
}

void console_writec(char c) {
    _console_writec(process_get_current_index(), c);
}

void console_dbg_writec(char c) {
    _console_writec(DBG_CONSOLE_INDEX, c);
}

static void _console_putc(int index, char c) {
    if (c == '\n') {
        go_to_next_line(index);
    } else {
        struct console *con = &consoles[index];
        u16 entry = vga_entry(c, con->bg_color, con->text_color);
        con->buffer[con->row * CONSOLE_COLUMNS + con->col] = entry;

        con->col++;
        if (con->col >= CONSOLE_COLUMNS) {
            go_to_next_line(index);
        }
    }
}

void console_putc(char c) {
    _console_putc(process_get_current_index(), c);
}

void console_dbg_putc(char c) {
    _console_putc(DBG_CONSOLE_INDEX, c);
}

static void _console_puts(int index, const char *str) {
    while (*str) {
        _console_putc(index, *str);
        str++;
    }
}

void console_puts(const char *str) {
    _console_puts(process_get_current_index(), str);
}

void console_dbg_puts(const char *str) {
    _console_puts(DBG_CONSOLE_INDEX, str);
}

static void _console_putb(int index, u8 b) {
    char str[3];
    str[2] = '\0';
    str[1] = hex_digits[b & 0xf];
    b >>= 4;
    str[0] = hex_digits[b & 0xf];
    _console_puts(index, str);
}

void console_putb(u8 b) {
    _console_putb(process_get_current_index(), b);
}

void console_dbg_putb(u8 b) {
    _console_putb(DBG_CONSOLE_INDEX, b);
}

static void _console_putw(int index, u16 w) {
    char str[5];
    str[4] = '\0';
    str[3] = hex_digits[w & 0xf];
    w >>= 4;
    str[2] = hex_digits[w & 0xf];
    w >>= 4;
    str[1] = hex_digits[w & 0xf];
    w >>= 4;
    str[0] = hex_digits[w & 0xf];
    _console_puts(index, str);
}

void console_putw(u16 w) {
    _console_putw(process_get_current_index(), w);
}

void console_dbg_putw(u16 w) {
    _console_putw(DBG_CONSOLE_INDEX, w);
}

static void _console_putdw(int index, u32 dw) {
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
    _console_puts(index, str);
}

void console_putdw(u32 dw) {
    _console_putdw(process_get_current_index(), dw);
}

void console_dbg_putdw(u32 dw) {
    _console_putdw(DBG_CONSOLE_INDEX, dw);
}

static void _console_putqw(int index, u64 qw) {
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
    _console_puts(index, str);
}

void console_putqw(u64 qw) {
    _console_putqw(process_get_current_index(), qw);
}

void console_dbg_putqw(u64 qw) {
    _console_putqw(DBG_CONSOLE_INDEX, qw);
}

static void _console_putp(int index, void *p) {
    _console_putdw(index, (u32)p);
}

void console_putp(void *p) {
    _console_putp(process_get_current_index(), p);
}

void console_dbg_putp(void *p) {
    _console_putp(DBG_CONSOLE_INDEX, p);
}

static void _console_putc_at(int index, char c, u8 col, u8 row) {
    struct console *con = &consoles[index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putc(c);
    con->col = old_col;
    con->row = old_row;
}

void console_putc_at(char c, u8 col, u8 row) {
    _console_putc_at(process_get_current_index(), c, col, row);
}

void console_dbg_putc_at(char c, u8 col, u8 row) {
    _console_putc_at(DBG_CONSOLE_INDEX, c, col, row);
}

static void _console_puts_at(int index, const char *str, u8 col, u8 row) {
    struct console *con = &consoles[index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_puts(str);
    con->col = old_col;
    con->row = old_row;
}

void console_puts_at(const char *str, u8 col, u8 row) {
    _console_puts_at(process_get_current_index(), str, col, row);
}

void console_dbg_puts_at(const char *str, u8 col, u8 row) {
    _console_puts_at(DBG_CONSOLE_INDEX, str, col, row);
}

static void _console_putb_at(int index, u8 b, u8 col, u8 row) {
    struct console *con = &consoles[index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putb(b);
    con->col = old_col;
    con->row = old_row;
}

void console_putb_at(u8 b, u8 col, u8 row) {
    _console_putb_at(process_get_current_index(), b, col, row);
}

void console_dbg_putb_at(u8 b, u8 col, u8 row) {
    _console_putb_at(DBG_CONSOLE_INDEX, b, col, row);
}

static void _console_putw_at(int index, u16 w, u8 col, u8 row) {
    struct console *con = &consoles[index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putw(w);
    con->col = old_col;
    con->row = old_row;
}

void console_putw_at(u16 w, u8 col, u8 row) {
    _console_putw_at(process_get_current_index(), w, col, row);
}

void console_dbg_putw_at(u16 w, u8 col, u8 row) {
    _console_putw_at(DBG_CONSOLE_INDEX, w, col, row);
}

static void _console_putdw_at(int index, u32 dw, u8 col, u8 row) {
    struct console *con = &consoles[index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putdw(dw);
    con->col = old_col;
    con->row = old_row;
}

void console_putdw_at(u32 dw, u8 col, u8 row) {
    _console_putdw_at(process_get_current_index(), dw, col, row);
}

void console_dbg_putdw_at(u32 dw, u8 col, u8 row) {
    _console_putdw_at(DBG_CONSOLE_INDEX, dw, col, row);
}

static void _console_putqw_at(int index, u64 qw, u8 col, u8 row) {
    struct console *con = &consoles[index];
    u8 old_col = con->col;
    u8 old_row = con->row;
    con->col = col;
    con->row = row;
    console_putqw(qw);
    con->col = old_col;
    con->row = old_row;
}

void console_putqw_at(u64 qw, u8 col, u8 row) {
    _console_putqw_at(process_get_current_index(), qw, col, row);
}

void console_dbg_putqw_at(u64 qw, u8 col, u8 row) {
    _console_putqw_at(DBG_CONSOLE_INDEX, qw, col, row);
}

static void _console_putp_at(int index, void *p, u8 col, u8 row) {
    _console_putdw_at(index, (u32)p, col, row);
}

void console_putp_at(void *p, u8 col, u8 row) {
    _console_putp_at(process_get_current_index(), p, col, row);
}

void console_dbg_putp_at(void *p, u8 col, u8 row) {
    _console_putp_at(DBG_CONSOLE_INDEX, p, col, row);
}

void console_repaint(void) {
    struct console *con = &consoles[current_console_index];
    for (int i = 0; i < CONSOLE_ROWS * CONSOLE_COLUMNS; i++) {
        vga_buffer[i] = con->buffer[i];
    }
}

void console_next(void) {
    current_console_index = (current_console_index + 1) % MAX_CONSOLES;
}

void console_prev(void) {
    current_console_index = (current_console_index + MAX_CONSOLES - 1) % MAX_CONSOLES;
}

void console_show_dbg(void) {
    current_console_index = DBG_CONSOLE_INDEX;
}

void console_key_press(u16 scancode) {
    if (current_console_index == DBG_CONSOLE_INDEX) {
        return;
    }
    struct key_event_buffer *keb = &consoles[current_console_index].keb;
    keb->events[keb->tail].scancode = scancode;
    keb->events[keb->tail].is_release = false;
    keb->tail = (keb->tail + 1) % KEY_EVENT_BUFFER_MAX_EVENTS;
    if (keb->tail == keb->head) {
        BUGCHECK("Key event buffer is full.");
    }
}

void console_key_release(u16 scancode) {
    if (current_console_index == DBG_CONSOLE_INDEX) {
        return;
    }
    struct key_event_buffer *keb = &consoles[current_console_index].keb;
    keb->events[keb->tail].scancode = scancode;
    keb->events[keb->tail].is_release = true;
    if (keb->tail == keb->head) {
        BUGCHECK("Key event buffer is full.");
    }
}

// struct key_event console_read_key_event(void) {

// }

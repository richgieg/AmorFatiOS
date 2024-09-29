#include <console.h>
#include <process.h>
#include <types.h>
#include <bugcheck.h>
#include <device/vga.h>

#define NUM_CONSOLES 6
#define NUM_CONSOLES_INC_DBG (NUM_CONSOLES + 1)
#define DBG_CONSOLE_INDEX (NUM_CONSOLES_INC_DBG - 1)
#define KEY_EVENT_BUFFER_MAX_EVENTS 256

#define SC_LEFT_CTRL 0x0014
#define SC_RIGHT_CTRL 0xe014
#define SC_LEFT_ALT 0x0011
#define SC_RIGHT_ALT 0xe011
#define SC_LEFT 0xe06b
#define SC_RIGHT 0xe074
#define SC_F1 0x0005
#define SC_F2 0x0006
#define SC_F3 0x0004
#define SC_F4 0x000c
#define SC_F5 0x0003
#define SC_F6 0x000b
#define SC_C 0x0021

static const char hex_digits[] = "0123456789ABCDEF";

struct key_event_buffer {
    struct key_event events[KEY_EVENT_BUFFER_MAX_EVENTS];
    u8 head;
    u8 tail;
};

// TODO: Use a linked list instead once we have a memory manager.
struct focus_list {
    int pids[64];
    int current_index;
};

struct console {
    u16 buffer[VGA_COLUMNS * VGA_ROWS];
    enum vga_color bg_color;
    enum vga_color text_color;
    u8 col;
    u8 row;
    struct key_event_buffer keb;
    struct focus_list focus_list;
} __attribute__((aligned(4096)));

static struct console consoles[NUM_CONSOLES_INC_DBG];
static int current_console_index;

static bool is_left_alt_down;
static bool is_right_alt_down;
static bool is_left_ctrl_down;
static bool is_right_ctrl_down;

// static int lock = 0;

static void console_repaint(void) {
    vga_copy(consoles[current_console_index].buffer);
}

void console_init(void) {
    enum vga_color bg_color = VGA_COLOR_CYAN;
    enum vga_color text_color = VGA_COLOR_BRIGHT_WHITE;
    u16 entry = vga_entry(0, bg_color, text_color);

    for (int i = 0; i < DBG_CONSOLE_INDEX; i++) {
        consoles[i].bg_color = bg_color;
        consoles[i].text_color = text_color;
        for (int j = 0; j < VGA_ROWS * VGA_COLUMNS; j++) {
            consoles[i].buffer[j] = entry;
        }
        consoles[i].focus_list.current_index = -1;
    }

    bg_color = VGA_COLOR_BLACK;
    text_color = VGA_COLOR_BRIGHT_WHITE;
    entry = vga_entry(0, bg_color, text_color);
    consoles[DBG_CONSOLE_INDEX].bg_color = bg_color;
    consoles[DBG_CONSOLE_INDEX].text_color = text_color;
    for (int j = 0; j < VGA_ROWS * VGA_COLUMNS; j++) {
        consoles[DBG_CONSOLE_INDEX].buffer[j] = entry;
    }

    console_repaint();
}

static inline void acquire_spin_lock(void) {
    // NOTE: Locking is temporarily disabled until I can fix the debug print
    // dealock issue.

    // __asm__(
    //     "retry:"
    //     "bts %[lock], 0;"
    //     "jc retry;"
    //     :
    //     : [lock] "m" (lock)
    //     : "memory"
    // );
}

static inline void release_spin_lock(void) {
    // lock = 0;
}

static void _console_clear(int index) {
    struct console *con = &consoles[index];
    u16 entry = vga_entry(0, con->bg_color, con->text_color);

    for (int i = 0; i < VGA_ROWS * VGA_COLUMNS; i++) {
        con->buffer[i] = entry;
    }

    acquire_spin_lock();
    if (index == current_console_index) {
        vga_copy(consoles[current_console_index].buffer);
    }
    release_spin_lock();

    con->col = 0;
    con->row = 0;
}

void console_clear(void) {
    _console_clear(process_get_console_index());
}

void console_dbg_clear(void) {
    _console_clear(DBG_CONSOLE_INDEX);
}

static enum vga_color _console_get_bg_color(int index) {
    struct console *con = &consoles[index];
    return con->bg_color;
}

enum vga_color console_get_bg_color(void) {
    return _console_get_bg_color(process_get_console_index());
}

enum vga_color console_dbg_get_bg_color() {
    return _console_get_bg_color(DBG_CONSOLE_INDEX);
}

static void _console_set_bg_color(int index, enum vga_color bg_color) {
    struct console *con = &consoles[index];
    con->bg_color = bg_color;
}

void console_set_bg_color(enum vga_color bg_color) {
    _console_set_bg_color(process_get_console_index(), bg_color);
}

void console_dbg_set_bg_color(enum vga_color bg_color) {
    _console_set_bg_color(DBG_CONSOLE_INDEX, bg_color);
}

static enum vga_color _console_get_text_color(int index) {
    struct console *con = &consoles[index];
    return con->text_color;
}

enum vga_color console_get_text_color(void) {
    return _console_get_text_color(process_get_console_index());
}

enum vga_color console_dbg_get_text_color() {
    return _console_get_text_color(DBG_CONSOLE_INDEX);
}

static void _console_set_text_color(int index, enum vga_color text_color) {
    struct console *con = &consoles[index];
    con->text_color = text_color;
}

void console_set_text_color(enum vga_color text_color) {
    _console_set_text_color(process_get_console_index(), text_color);
}

void console_dbg_set_text_color(enum vga_color text_color) {
    _console_set_text_color(DBG_CONSOLE_INDEX, text_color);
}

static u8 _console_get_column(int index) {
    return consoles[index].col;
}

u8 console_get_column(void) {
    return _console_get_column(process_get_console_index());
}

static u8 _console_get_row(int index) {
    return consoles[index].row;
}

u8 console_get_row(void) {
    return _console_get_row(process_get_console_index());
}

static void _console_set_pos(int index, u8 col, u8 row) {
    struct console *con = &consoles[index];
    con->col = col;
    con->row = row;
}

void console_set_pos(u8 col, u8 row) {
    _console_set_pos(process_get_console_index(), col, row);
}

void console_dbg_set_pos(u8 col, u8 row) {
    _console_set_pos(DBG_CONSOLE_INDEX, col, row);
}

static void scroll_one_line(int index) {
    struct console *con = &consoles[index];

    for (int i = 0; i < (VGA_ROWS - 1) * VGA_COLUMNS; i++) {
        con->buffer[i] = con->buffer[i + VGA_COLUMNS];
    }

    for (int i = 0; i < VGA_COLUMNS; i++) {
        con->buffer[(VGA_ROWS - 1) * VGA_COLUMNS + i] = vga_entry(0, con->bg_color, con->text_color);
    }

    acquire_spin_lock();
    if (index == current_console_index) {
        vga_copy(consoles[current_console_index].buffer);
    }
    release_spin_lock();
}

static void go_to_next_line(int index) {
    struct console *con = &consoles[index];
    con->col = 0;
    con->row++;
    if (con->row >= VGA_ROWS) {
        con->row = VGA_ROWS - 1;
        scroll_one_line(index);
    }
}

static void handle_backspace(int index) {
    struct console *con = &consoles[index];
    if (con->col > 0) {
        con->col--;
    } else if (con->row > 0) {
        con->col = VGA_COLUMNS - 1;
        con->row--;
    }
    u16 entry = vga_entry('\0', con->bg_color, con->text_color);
    con->buffer[con->row * VGA_COLUMNS + con->col] = entry;

    acquire_spin_lock();
    if (index == current_console_index) {
        vga_write(con->row * VGA_COLUMNS + con->col, entry);
    }
    release_spin_lock();
}

static void _console_writec(int index, char c) {
    struct console *con = &consoles[index];
    u16 entry = vga_entry(c, con->bg_color, con->text_color);
    con->buffer[con->row * VGA_COLUMNS + con->col] = entry;

    acquire_spin_lock();
    if (index == current_console_index) {
        vga_write(con->row * VGA_COLUMNS + con->col, entry);
    }
    release_spin_lock();

    con->col++;
    if (con->col >= VGA_COLUMNS) {
        go_to_next_line(index);
    }
}

void console_writec(char c) {
    _console_writec(process_get_console_index(), c);
}

void console_dbg_writec(char c) {
    _console_writec(DBG_CONSOLE_INDEX, c);
}

static void _console_putc(int index, char c) {
    if (c == '\n') {
        go_to_next_line(index);
    } else if (c == '\b') {
        handle_backspace(index);
    } else {
        struct console *con = &consoles[index];
        u16 entry = vga_entry(c, con->bg_color, con->text_color);
        con->buffer[con->row * VGA_COLUMNS + con->col] = entry;

        acquire_spin_lock();
        if (index == current_console_index) {
            vga_write(con->row * VGA_COLUMNS + con->col, entry);
        }
        release_spin_lock();

        con->col++;
        if (con->col >= VGA_COLUMNS) {
            go_to_next_line(index);
        }
    }
}

void console_putc(char c) {
    _console_putc(process_get_console_index(), c);
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
    _console_puts(process_get_console_index(), str);
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
    _console_putb(process_get_console_index(), b);
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
    _console_putw(process_get_console_index(), w);
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
    _console_putdw(process_get_console_index(), dw);
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
    _console_putqw(process_get_console_index(), qw);
}

void console_dbg_putqw(u64 qw) {
    _console_putqw(DBG_CONSOLE_INDEX, qw);
}

static void _console_putp(int index, void *p) {
    _console_putdw(index, (u32)p);
}

void console_putp(void *p) {
    _console_putp(process_get_console_index(), p);
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
    _console_putc_at(process_get_console_index(), c, col, row);
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
    _console_puts_at(process_get_console_index(), str, col, row);
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
    _console_putb_at(process_get_console_index(), b, col, row);
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
    _console_putw_at(process_get_console_index(), w, col, row);
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
    _console_putdw_at(process_get_console_index(), dw, col, row);
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
    _console_putqw_at(process_get_console_index(), qw, col, row);
}

void console_dbg_putqw_at(u64 qw, u8 col, u8 row) {
    _console_putqw_at(DBG_CONSOLE_INDEX, qw, col, row);
}

static void _console_putp_at(int index, void *p, u8 col, u8 row) {
    _console_putdw_at(index, (u32)p, col, row);
}

void console_putp_at(void *p, u8 col, u8 row) {
    _console_putp_at(process_get_console_index(), p, col, row);
}

void console_dbg_putp_at(void *p, u8 col, u8 row) {
    _console_putp_at(DBG_CONSOLE_INDEX, p, col, row);
}

void console_next(void) {
    current_console_index = (current_console_index + 1) % NUM_CONSOLES_INC_DBG;
    console_repaint();
}

void console_prev(void) {
    current_console_index = (
        current_console_index + NUM_CONSOLES_INC_DBG - 1) % NUM_CONSOLES_INC_DBG;
    console_repaint();
}

void console_switch_to(int index) {
    current_console_index = index;
    console_repaint();
}

void console_switch_to_dbg(void) {
    current_console_index = DBG_CONSOLE_INDEX;
    console_repaint();
}

static void buffer_key_press(u16 scancode) {
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

static void buffer_key_release(u16 scancode) {
    if (current_console_index == DBG_CONSOLE_INDEX) {
        return;
    }
    struct key_event_buffer *keb = &consoles[current_console_index].keb;
    keb->events[keb->tail].scancode = scancode;
    keb->events[keb->tail].is_release = true;
    keb->tail = (keb->tail + 1) % KEY_EVENT_BUFFER_MAX_EVENTS;
    if (keb->tail == keb->head) {
        BUGCHECK("Key event buffer is full.");
    }
}

// REMINDER: This runs in interrupt context!
void console_handle_key_press(u16 scancode) {
    switch (scancode) {
        case SC_LEFT_CTRL:
            is_left_ctrl_down = true;
            buffer_key_press(scancode);
            break;
        case SC_RIGHT_CTRL:
            is_right_ctrl_down = true;
            buffer_key_press(scancode);
            break;
        case SC_LEFT_ALT:
            is_left_alt_down = true;
            buffer_key_press(scancode);
            break;
        case SC_RIGHT_ALT:
            is_right_alt_down = true;
            buffer_key_press(scancode);
            break;
        case SC_F1:
            if (is_left_alt_down || is_right_alt_down) {
                console_switch_to(0);
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_F2:
            if (is_left_alt_down || is_right_alt_down) {
                console_switch_to(1);
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_F3:
            if (is_left_alt_down || is_right_alt_down) {
                console_switch_to(2);
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_F4:
            if (is_left_alt_down || is_right_alt_down) {
                console_switch_to(3);
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_F5:
            if (is_left_alt_down || is_right_alt_down) {
                console_switch_to(4);
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_F6:
            if (is_left_alt_down || is_right_alt_down) {
                console_switch_to(5);
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_LEFT:
            if (is_left_alt_down || is_right_alt_down) {
                console_prev();
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_RIGHT:
            if (is_left_alt_down || is_right_alt_down) {
                console_next();
                if (is_left_alt_down) {
                    buffer_key_press(SC_LEFT_ALT);
                }
                if (is_right_alt_down) {
                    buffer_key_press(SC_RIGHT_ALT);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        case SC_C:
            if (is_left_ctrl_down || is_right_ctrl_down) {
                if (current_console_index != DBG_CONSOLE_INDEX) {
                    struct console *con = &consoles[current_console_index];
                    int pid = con->focus_list.pids[con->focus_list.current_index];
                    process_send_kill_signal(pid);
                }
            } else {
                buffer_key_press(scancode);
            }
            break;
        default:
            buffer_key_press(scancode);
            break;
    }
}

// REMINDER: This runs in interrupt context!
void console_handle_key_release(u16 scancode) {
    switch (scancode) {
        case SC_LEFT_CTRL:
            is_left_ctrl_down = false;
            buffer_key_release(scancode);
            break;
        case SC_RIGHT_CTRL:
            is_right_ctrl_down = false;
            buffer_key_release(scancode);
            break;
        case SC_LEFT_ALT:
            is_left_alt_down = false;
            buffer_key_release(scancode);
            break;
        case SC_RIGHT_ALT:
            is_right_alt_down = false;
            buffer_key_release(scancode);
            break;
        default:
            buffer_key_release(scancode);
            break;
    }
}

void console_read_key_event(struct key_event *ke) {
    struct console *con = &consoles[process_get_console_index()];
    int pid = process_get_current_pid();
    if (con->focus_list.pids[con->focus_list.current_index] != pid) {
        process_switch(PROCESS_STATE_WAITING_FOR_KEY_EVENT);
    }
    if (con->keb.head == con->keb.tail) {
        process_switch(PROCESS_STATE_WAITING_FOR_KEY_EVENT);
    }
    *ke = con->keb.events[con->keb.head];
    con->keb.head = (con->keb.head + 1) % KEY_EVENT_BUFFER_MAX_EVENTS;
}

bool console_has_key_event_for_process(int index, int pid) {
    struct console *con = &consoles[index];
    if (con->focus_list.pids[con->focus_list.current_index] != pid) {
        return false;
    }
    return con->keb.head != con->keb.tail;
}

int console_get_num_columns(void) {
    return VGA_COLUMNS;
}

int console_get_num_rows(void) {
    return VGA_ROWS;
}

int console_get_num_consoles(void) {
    return NUM_CONSOLES;
}

/**
 * Adds a pid to the end of a console's focus list. After this operation, the
 * associated process will have focus on the console.
 */
static void add_pid_to_focus_list(int index, int pid) {
    struct console *con = &consoles[index];
    int max_entries = sizeof(con->focus_list.pids) / sizeof(con->focus_list.pids[0]);
    con->focus_list.current_index++;
    if (con->focus_list.current_index == max_entries) {
        BUGCHECK("Console focus list overflow.");
    }
    con->focus_list.pids[con->focus_list.current_index] = pid;
}

/**
 * Removes a pid from a console's focus list. If the process currently has
 * focus then focus will be transferred to the pid that is immediately before
 * it in the focus list.
 */
static void remove_pid_from_focus_list(int index, int pid) {
    struct console *con = &consoles[index];
    for (int i = 0; i <= con->focus_list.current_index; i++) {
        if (con->focus_list.pids[i] == pid) {
            // Remove entry by shifting following entries over by one.
            for (int j = i; j < con->focus_list.current_index; j++) {
                con->focus_list.pids[j] = con->focus_list.pids[j + 1];
            }
            con->focus_list.current_index--;
            break;
        }
    }
}

void console_handle_process_start(int index, int pid) {
    add_pid_to_focus_list(index, pid);
}

void console_handle_process_exit(int index, int pid) {
    remove_pid_from_focus_list(index, pid);
}

void console_handle_process_kill(int index, int pid) {
    struct console *con = &consoles[index];
    if (con->focus_list.pids[con->focus_list.current_index] == pid) {
        _console_putc(index, '\n');
    }
    console_handle_process_exit(index, pid);
}

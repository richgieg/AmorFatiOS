// Reference: https://en.wikipedia.org/wiki/VGA_text_mode

#include "vga.h"
#include "port.h"

#define VGA_ADDRESS 0xb8000
#define ROWS 25
#define COLUMNS 80

static const char hex_digits[] = "0123456789ABCDEF";

static enum vga_color cur_bg_color = VGA_COLOR_CYAN;
static enum vga_color cur_text_color = VGA_COLOR_BRIGHT_WHITE;

static uint8_t cur_col = 0;
static uint8_t cur_row = 0;

void vga_init(void) {
    // Enable usage of all 16 background colors.
    // This ensures that attribute bit 7 is used for color instead of blink.
    // https://www.reddit.com/r/osdev/comments/70fcig/blinking_text/

    // Read I/O Address 0x3da to reset index/data flip-flop
    inb(0x3da);
    // Write index 0x30 to 0x3c0 to set register index to 0x30
    outb(0x3c0, 0x30);
    // Read from 0x03c1 to get register contents
    uint8_t flags = inb(0x3c1);
    // Unset Bit 3 to disable Blink
    flags &= 0xf7;
    // Write to 0x3c0 to update register with changed value
    outb(0x3c0, flags);

    // Disable the cursor.
    outb(0x3d4, 0x0a);
    outb(0x3d5, 0x20);

    vga_clear();
}

static inline uint16_t vga_entry(
    char character, enum vga_color bg_color, enum vga_color text_color
) {
    uint8_t attributes = bg_color << 4 | text_color;
    return (uint8_t)character | (uint16_t)attributes << 8;
}

void vga_clear(void) {
    volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
    uint16_t entry = vga_entry(0, cur_bg_color, cur_text_color);
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        vga_buffer[i] = entry;
    }
}

enum vga_color vga_get_bg_color() {
    return cur_bg_color;
}

void vga_set_bg_color(enum vga_color bg_color) {
    cur_bg_color = bg_color;
}

enum vga_color vga_get_text_color() {
    return cur_text_color;
}

void vga_set_text_color(enum vga_color text_color) {
    cur_text_color = text_color;
}

static void scroll_one_line(void) {
    volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
    for (int i = 0; i < (ROWS - 1) * COLUMNS; i++) {
        vga_buffer[i] = vga_buffer[i + COLUMNS];
    }
    for (int i = 0; i < COLUMNS; i++) {
        vga_buffer[(ROWS - 1) * COLUMNS + i] = vga_entry(0, cur_bg_color, cur_text_color);
    }
}

static void go_to_next_line(void) {
    cur_col = 0;
    cur_row++;
    if (cur_row >= ROWS) {
        cur_row = ROWS - 1;
        scroll_one_line();
    }
}

void vga_writec(char c) {
    volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
    vga_buffer[cur_row * COLUMNS + cur_col] = vga_entry(c, cur_bg_color, cur_text_color);
    cur_col++;
    if (cur_col >= COLUMNS) {
        go_to_next_line();
    }
}

void vga_putc(char c) {
    if (c == '\n') {
        go_to_next_line();
    } else {
        volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
        vga_buffer[cur_row * COLUMNS + cur_col] = vga_entry(c, cur_bg_color, cur_text_color);
        cur_col++;
        if (cur_col >= COLUMNS) {
            go_to_next_line();
        }
    }
}

void vga_puts(const char *str) {
    while (*str) {
        vga_putc(*str);
        str++;
    }
}

void vga_putb(uint8_t b) {
    char str[3];
    str[2] = '\0';
    str[1] = hex_digits[b & 0xf];
    b >>= 4;
    str[0] = hex_digits[b & 0xf];
    vga_puts(str);
}

void vga_putw(uint16_t w) {
    char str[5];
    str[4] = '\0';
    str[3] = hex_digits[w & 0xf];
    w >>= 4;
    str[2] = hex_digits[w & 0xf];
    w >>= 4;
    str[1] = hex_digits[w & 0xf];
    w >>= 4;
    str[0] = hex_digits[w & 0xf];
    vga_puts(str);
}

void vga_putdw(uint32_t dw) {
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
    vga_puts(str);
}

void vga_putqw(uint64_t qw) {
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
    vga_puts(str);
}

void vga_putc_at(char c, uint8_t col, uint8_t row) {
    uint8_t old_col = cur_col;
    uint8_t old_row = cur_row;
    cur_col = col;
    cur_row = row;
    vga_putc(c);
    cur_col = old_col;
    cur_row = old_row;
}

void vga_puts_at(const char *str, uint8_t col, uint8_t row) {
    uint8_t old_col = cur_col;
    uint8_t old_row = cur_row;
    cur_col = col;
    cur_row = row;
    vga_puts(str);
    cur_col = old_col;
    cur_row = old_row;
}

void vga_putb_at(uint8_t b, uint8_t col, uint8_t row) {
    uint8_t old_col = cur_col;
    uint8_t old_row = cur_row;
    cur_col = col;
    cur_row = row;
    vga_putb(b);
    cur_col = old_col;
    cur_row = old_row;
}

void vga_putw_at(uint16_t w, uint8_t col, uint8_t row) {
    uint8_t old_col = cur_col;
    uint8_t old_row = cur_row;
    cur_col = col;
    cur_row = row;
    vga_putw(w);
    cur_col = old_col;
    cur_row = old_row;
}

void vga_putdw_at(uint32_t dw, uint8_t col, uint8_t row) {
    uint8_t old_col = cur_col;
    uint8_t old_row = cur_row;
    cur_col = col;
    cur_row = row;
    vga_putdw(dw);
    cur_col = old_col;
    cur_row = old_row;
}

void vga_putqw_at(uint64_t qw, uint8_t col, uint8_t row) {
    uint8_t old_col = cur_col;
    uint8_t old_row = cur_row;
    cur_col = col;
    cur_row = row;
    vga_putqw(qw);
    cur_col = old_col;
    cur_row = old_row;
}

void vga_dump_chars(void) {
    uint8_t x = 0;
    uint8_t y = 0;
    char c = 0;
    for (int i = 0; i < 256; i++) {
        vga_putc_at(c, x, y);
        c++;
        x++;
        if (x >= COLUMNS) {
            x = 0;
            y++;
        }
    }
}

// Reference: https://en.wikipedia.org/wiki/VGA_text_mode

#include "vga.h"
#include "port.h"

#define VGA_ADDRESS 0xb8000
#define ROWS 25
#define COLUMNS 80

enum vga_color bg_color = VGA_COLOR_CYAN;
enum vga_color text_color = VGA_COLOR_BRIGHT_WHITE;

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
    uint16_t entry = vga_entry(0, bg_color, text_color);
    for (int i = 0; i < ROWS * COLUMNS; i++) {
        vga_buffer[i] = entry;
    }
}

void vga_putc_at(char c, uint8_t x, uint8_t y) {
    volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
    vga_buffer[y * COLUMNS + x] = vga_entry(c, bg_color, text_color);
}

void vga_puts_at(const char *str, uint8_t x, uint8_t y) {
    volatile uint16_t *vga_buffer = (volatile uint16_t *)VGA_ADDRESS;
    while (*str) {
        vga_buffer[y * COLUMNS + x] = vga_entry(*str, bg_color, text_color);
        str++;
        x++;
        if (x >= COLUMNS) {
            x = 0;
            y++;
        }
    }
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

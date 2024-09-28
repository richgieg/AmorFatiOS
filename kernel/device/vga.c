// Reference: https://en.wikipedia.org/wiki/VGA_text_mode

#include <device/vga.h>
#include <port.h>

#define VGA_ADDRESS 0xb8000

static volatile u16 *vga_buffer = (volatile u16 *)VGA_ADDRESS;

void vga_init(void) {
    // Enable usage of all 16 background colors.
    // This ensures that attribute bit 7 is used for color instead of blink.
    // https://www.reddit.com/r/osdev/comments/70fcig/blinking_text/

    // Read I/O Address 0x3da to reset index/data flip-flop
    inb(0x3da);
    // Write index 0x30 to 0x3c0 to set register index to 0x30
    outb(0x3c0, 0x30);
    // Read from 0x03c1 to get register contents
    u8 flags = inb(0x3c1);
    // Unset Bit 3 to disable Blink
    flags &= 0xf7;
    // Write to 0x3c0 to update register with changed value
    outb(0x3c0, flags);

    // Disable the cursor.
    outb(0x3d4, 0x0a);
    outb(0x3d5, 0x20);
}

void vga_copy(void *src) {
    u64 *dst = (u64 *)vga_buffer;
    int length = VGA_ROWS * VGA_COLUMNS * sizeof(u16) / sizeof(u64);
    for (int i = 0; i < length; i++) {
        dst[i] = ((u64 *)src)[i];
    }
}

void vga_write(int index, u16 entry) {
    vga_buffer[index] = entry;
}

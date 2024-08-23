// Source: https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/keyboard.html

#include <stdint.h>

// Standard port input/output operations
// Source: https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access

static inline void outb(uint16_t port, uint8_t val) {
    __asm__("outb %1, %0" : : "a"(val), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    __asm__("inb %0, %1" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

// Source: https://curtsinger.cs.grinnell.edu/teaching/2022S/CSC395/kernel/keyboard.html

#ifndef PORT_H
#define PORT_H

#include "types.h"

// Standard port input/output operations
// Source: https://wiki.osdev.org/Inline_Assembly/Examples#I.2FO_access

static inline void outb(u16 port, u8 val) {
    __asm__("outb %1, %0" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__("inb %0, %1" : "=a"(ret) : "Nd"(port));
    return ret;
}

static inline void io_wait(void) {
    outb(0x80, 0);
}

#endif

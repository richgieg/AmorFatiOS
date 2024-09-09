#ifndef CONSOLE_H
#define CONSOLE_H

#include <types.h>

struct key_event {
    u16 scancode;
    bool is_release;
};

#endif

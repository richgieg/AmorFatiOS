#ifndef IDT_H
#define IDT_H

#include "types.h"

void idt_init(void);
void idt_set_irq_handler(u8 irq, void (*handler)());

#endif

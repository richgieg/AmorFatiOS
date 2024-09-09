#ifndef SYSCALL_H
#define SYSCALL_H

#include <console.h>

void sys_console_read_key_event(struct key_event *ke);
void sys_console_putdw_at(u32 dw, u8 col, u8 row);
void sys_process_create(void (*start)());

#endif

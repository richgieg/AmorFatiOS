#ifndef SYS_H
#define SYS_H

#include <console.h>

void sys_process_create(void (*start)());

void sys_console_clear(void);
void sys_console_get_bg_color(enum vga_color *bg_color);
void sys_console_set_bg_color(enum vga_color bg_color);
void sys_console_get_text_color(enum vga_color *text_color);
void sys_console_set_text_color(enum vga_color text_color);
void sys_console_set_pos(u8 col, u8 row);
void sys_console_writec(char c);
void sys_console_putc(char c);
void sys_console_puts(const char *str);
void sys_console_putb(u8);
void sys_console_putw(u16 w);
void sys_console_putdw(u32 dw);
void sys_console_putqw(u64 qw);
void sys_console_putp(void *p);
void sys_console_putc_at(char c, u8 col, u8 row);
void sys_console_puts_at(const char *str, u8 col, u8 row);
void sys_console_putb_at(u8 b, u8 col, u8 row);
void sys_console_putw_at(u16 w, u8 col, u8 row);
void sys_console_putdw_at(u32 dw, u8 col, u8 row);
void sys_console_putqw_at(u64 qw, u8 col, u8 row);
void sys_console_putp_at(void *p, u8 col, u8 row);
void sys_console_read_key_event(struct key_event *ke);
void sys_console_get_num_columns(int *columns);
void sys_console_get_num_rows(int *rows);

#endif

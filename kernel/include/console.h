#ifndef CONSOLE_H
#define CONSOLE_H

#include <device/vga.h>

struct key_event {
    u16 scancode;
    bool is_release;
};

void console_init(void);
void console_clear(void);
void console_dbg_clear(void);
enum vga_color console_get_bg_color();
enum vga_color console_dbg_get_bg_color();
void console_set_bg_color(enum vga_color bg_color);
void console_dbg_set_bg_color(enum vga_color bg_color);
enum vga_color console_get_text_color();
enum vga_color console_dbg_get_text_color();
void console_set_text_color(enum vga_color text_color);
void console_dbg_set_text_color(enum vga_color text_color);
void console_set_pos(u8 col, u8 row);
void console_dbg_set_pos(u8 col, u8 row);
void console_writec(char c);
void console_dbg_writec(char c);
void console_putc(char c);
void console_dbg_putc(char c);
void console_puts(const char *str);
void console_dbg_puts(const char *str);
void console_putb(u8);
void console_dbg_putb(u8);
void console_putw(u16 w);
void console_dbg_putw(u16 w);
void console_putdw(u32 dw);
void console_dbg_putdw(u32 dw);
void console_putqw(u64 qw);
void console_dbg_putqw(u64 qw);
void console_putp(void *p);
void console_dbg_putp(void *p);
void console_putc_at(char c, u8 col, u8 row);
void console_dbg_putc_at(char c, u8 col, u8 row);
void console_puts_at(const char *str, u8 col, u8 row);
void console_dbg_puts_at(const char *str, u8 col, u8 row);
void console_putb_at(u8 b, u8 col, u8 row);
void console_dbg_putb_at(u8 b, u8 col, u8 row);
void console_putw_at(u16 w, u8 col, u8 row);
void console_dbg_putw_at(u16 w, u8 col, u8 row);
void console_putdw_at(u32 dw, u8 col, u8 row);
void console_dbg_putdw_at(u32 dw, u8 col, u8 row);
void console_putqw_at(u64 dw, u8 col, u8 row);
void console_dbg_putqw_at(u64 dw, u8 col, u8 row);
void console_putp_at(void *p, u8 col, u8 row);
void console_dbg_putp_at(void *p, u8 col, u8 row);
void console_repaint(void);
void console_next(void);
void console_prev(void);
void console_show_dbg(void);
void console_key_press(u16 scancode);
void console_key_release(u16 scancode);
struct key_event console_read_key_event(void);
bool console_has_key_event(int index);
int console_get_num_columns(void);
int console_get_num_rows(void);

#endif